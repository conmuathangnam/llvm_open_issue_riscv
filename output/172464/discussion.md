# Serialization bug with pack indexing

**Author:** zyn0217
**URL:** https://github.com/llvm/llvm-project/issues/172464
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-12-23T10:48:31Z

## Body

The following test case fails with clang trunk:

```cpp

// RUN: rm -rf %t
// RUN: split-file %s %t

// RUN: %clang_cc1 -std=c++2c -x c++-header %t/test.h -emit-pch -o %t/test.pch
// RUN: %clang_cc1 -std=c++2c -x c++ %t/test.cpp -include-pch %t/test.pch

//--- test.h
template <class... Ts> struct _TypeInfo {
	template <int id> using type = Ts...[id];
	static constexpr int count = sizeof...(Ts);
};
using TypeInfo = _TypeInfo<int>;

TypeInfo::type<0> a;

//--- test.cpp
int main() {
  TypeInfo::type<0> a;
}


```

I can see false positive diagnostics:

```
llvm-project-Build/Build/tools/clang/test/PCH/Output/pack-indexing-2.cpp.tmp/test.h:2:39: error: invalid index 0 for pack 'Ts' of size 0
# |     2 |         template <int id> using type = Ts...[id];
# |       |                                              ^
```

This was first reported in https://github.com/clangd/clangd/issues/2529

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Younan Zhang (zyn0217)

<details>
The following test case fails with clang trunk:

```cpp

// RUN: rm -rf %t
// RUN: split-file %s %t

// RUN: %clang_cc1 -std=c++2c -x c++-header %t/test.h -emit-pch -o %t/test.pch
// RUN: %clang_cc1 -std=c++2c -x c++ %t/test.cpp -include-pch %t/test.pch

//--- test.h
template &lt;class... Ts&gt; struct _TypeInfo {
	template &lt;int id&gt; using type = Ts...[id];
	static constexpr int count = sizeof...(Ts);
};
using TypeInfo = _TypeInfo&lt;int&gt;;

TypeInfo::type&lt;0&gt; a;

//--- test.cpp
int main() {
  TypeInfo::type&lt;0&gt; a;
}


```

I can see false positive diagnostics:

```
llvm-project-Build/Build/tools/clang/test/PCH/Output/pack-indexing-2.cpp.tmp/test.h:2:39: error: invalid index 0 for pack 'Ts' of size 0
# |     2 |         template &lt;int id&gt; using type = Ts...[id];
# |       |                                              ^
```

This was first reported in https://github.com/clangd/clangd/issues/2529
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Younan Zhang (zyn0217)

<details>
The following test case fails with clang trunk:

```cpp

// RUN: rm -rf %t
// RUN: split-file %s %t

// RUN: %clang_cc1 -std=c++2c -x c++-header %t/test.h -emit-pch -o %t/test.pch
// RUN: %clang_cc1 -std=c++2c -x c++ %t/test.cpp -include-pch %t/test.pch

//--- test.h
template &lt;class... Ts&gt; struct _TypeInfo {
	template &lt;int id&gt; using type = Ts...[id];
	static constexpr int count = sizeof...(Ts);
};
using TypeInfo = _TypeInfo&lt;int&gt;;

TypeInfo::type&lt;0&gt; a;

//--- test.cpp
int main() {
  TypeInfo::type&lt;0&gt; a;
}


```

I can see false positive diagnostics:

```
llvm-project-Build/Build/tools/clang/test/PCH/Output/pack-indexing-2.cpp.tmp/test.h:2:39: error: invalid index 0 for pack 'Ts' of size 0
# |     2 |         template &lt;int id&gt; using type = Ts...[id];
# |       |                                              ^
```

This was first reported in https://github.com/clangd/clangd/issues/2529
</details>


---

