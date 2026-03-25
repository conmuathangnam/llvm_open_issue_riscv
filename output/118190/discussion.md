# [Clang] Variable template partial specialization cannot specialize primary template with `auto` NTTP

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/118190
**Status:** Closed
**Labels:** clang:frontend, constexpr, diverges-from:gcc, diverges-from:msvc, diverges-from:edg
**Closed Date:** 2025-08-09T23:03:03Z

## Body

Reproducer:
```c++
template <auto> constexpr bool b = false;
template <int I> constexpr bool b<I> = true; 
```

Error:
```
<source>:2:33: error: variable template partial specialization does not specialize any template argument; to define the primary template, remove the template argument list
    2 | template <int I> constexpr bool b<I> = true;
      |                                 ^~~~
```

Possibly related: #42421.

See Compiler Explorer: https://godbolt.org/z/nEdbcKqWs

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
Reproducer:
```c++
template &lt;auto&gt; constexpr bool b = false;
template &lt;int I&gt; constexpr bool b&lt;I&gt; = true; 
```

Error:
```
&lt;source&gt;:2:33: error: variable template partial specialization does not specialize any template argument; to define the primary template, remove the template argument list
    2 | template &lt;int I&gt; constexpr bool b&lt;I&gt; = true;
      |                                 ^~~~
```

Possibly related: #<!-- -->42421.

See Compiler Explorer: https://godbolt.org/z/nEdbcKqWs
</details>


---

### Comment 2 - shafik

edg also agrees w/ gcc/MSVS: https://godbolt.org/z/MqE54KnfM

---

### Comment 3 - hubert-reinterpretcast

This is a bug. The equivalent class template partial specialization case does work with Clang.

---

