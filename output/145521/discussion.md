# nested class with out-of-class-definition and requires clause fails to compile

**Author:** bernd5
**URL:** https://github.com/llvm/llvm-project/issues/145521
**Status:** Closed
**Labels:** c++20, clang:frontend, concepts, confirmed, diverges-from:gcc, regression:18
**Closed Date:** 2025-07-08T11:01:02Z

## Body

The following code fails to compile:

```c++
template <typename X>
concept is_valid = true;

template<typename T>
class Nesting
{
public:
    template<typename Q> requires is_valid<Q>
    class Inner;
};

template<typename T>
template<typename Q> requires is_valid<Q>
class Nesting<T>::Inner {};
```

with the error message:

```
<source>:13:31: error: requires clause differs in template redeclaration
   13 | template<typename Q> requires is_valid<Q>
      |                               ^
<source>:8:35: note: previous template declaration is here
    8 |     template<typename Q> requires is_valid<Q>
      |                                   ^
1 error generated.
Compiler returned: 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Bernd Baumanns (bernd5)

<details>
The following code fails to compile:

```c++
template &lt;typename X&gt;
concept is_valid = true;

template&lt;typename T&gt;
class Nesting
{
public:
    template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
    class Inner;
};

template&lt;typename T&gt;
template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
class Nesting&lt;T&gt;::Inner {};
```

with the error message:

```
&lt;source&gt;:13:31: error: requires clause differs in template redeclaration
   13 | template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
      |                               ^
&lt;source&gt;:8:35: note: previous template declaration is here
    8 |     template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
      |                                   ^
1 error generated.
Compiler returned: 1
```
</details>


---

### Comment 2 - EugeneZelenko

GCC compiles this code: https://godbolt.org/z/1aGMz9j4s

---

### Comment 3 - shafik

This looks like a bug CC @erichkeane @cor3ntin 

---

### Comment 4 - shafik

Looks like this started in clang-18: https://godbolt.org/z/7r5zPqvjP

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-c-20

Author: Bernd Baumanns (bernd5)

<details>
The following code fails to compile:

```c++
template &lt;typename X&gt;
concept is_valid = true;

template&lt;typename T&gt;
class Nesting
{
public:
    template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
    class Inner;
};

template&lt;typename T&gt;
template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
class Nesting&lt;T&gt;::Inner {};
```

with the error message:

```
&lt;source&gt;:13:31: error: requires clause differs in template redeclaration
   13 | template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
      |                               ^
&lt;source&gt;:8:35: note: previous template declaration is here
    8 |     template&lt;typename Q&gt; requires is_valid&lt;Q&gt;
      |                                   ^
1 error generated.
Compiler returned: 1
```
</details>


---

### Comment 6 - erichkeane

Agreed this looks like a bug, our canonicalization of the requirement isn't doing the right thing here.  We've got a few patches in flight that fix things like this (corentin has been working on a big one!), so perhaps this will get done by one of those.

---

### Comment 7 - shafik

@erichkeane git bisect points to 98191d7c160cc982ac3f4db234da080e2bd8a192

---

### Comment 8 - erichkeane

I suspected it was that or one like it.  That has unfortunately/fortunately been modified about a dozen times since that was written.  We've been working around a lot of that stuff for quite a while (and note that before then, we weren't really checking much at all for conflicting), so I'm not surprised.  Comment above still stands.

---

### Comment 9 - cor3ntin

@zyn0217 

---

