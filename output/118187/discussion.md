# Cannot call lambda defined in immediate lambda

**Author:** LHLaurini
**URL:** https://github.com/llvm/llvm-project/issues/118187
**Status:** Closed
**Labels:** clang:frontend, confirmed, lambda, regression:19, diverges-from:gcc, diverges-from:msvc
**Closed Date:** 2025-11-23T03:12:00Z

## Body

The following code compiles on both GCC and MSVC, but is refused by Clang ([Godbolt link](https://godbolt.org/z/M7KdM36YE)):
```c++
void func_1(auto)
{
    [](auto) consteval
    {
        [](auto)
        {
        }(123);
    };
}

auto func_2()
{
    func_1(123);
}
```

The error message:
```
<source>:6:9: error: no matching function for call to object of type '(lambda at <source>:6:9)'
    6 |         [](auto)
      |         ^~~~~~~~
    7 |         {
      |         ~
    8 |         }(123);
      |         ~
<source>:5:5: note: while substituting into a lambda expression here
    5 |     {
      |     ^
<source>:14:5: note: in instantiation of function template specialization 'func_1<int>' requested here
   14 |     func_1(123);
      |     ^
<source>:6:9: note: candidate template ignored: couldn't infer template argument 'auto:1'
    6 |         [](auto)
      |         ^
1 error generated.
```

From my experimentation:
- `func_1` must be a function template;
- The outer lambda must be immediate;
- The outer lambda must be either polymorphic or a lambda template;
- The inner lambda must be either polymorphic or a lambda template.

Note: the code from which I wrote this test-case is almost the same where I originally found #117676, so there could be some relation (or not).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Luiz Henrique Laurini (LHLaurini)

<details>
The following code compiles on both GCC and MSVC, but is refused by Clang ([Godbolt link](https://godbolt.org/z/M7KdM36YE)):
```c++
void func_1(auto)
{
    [](auto) consteval
    {
        [](auto)
        {
        }(123);
    };
}

auto func_2()
{
    func_1(123);
}
```

The error message:
```
&lt;source&gt;:6:9: error: no matching function for call to object of type '(lambda at &lt;source&gt;:6:9)'
    6 |         [](auto)
      |         ^~~~~~~~
    7 |         {
      |         ~
    8 |         }(123);
      |         ~
&lt;source&gt;:5:5: note: while substituting into a lambda expression here
    5 |     {
      |     ^
&lt;source&gt;:14:5: note: in instantiation of function template specialization 'func_1&lt;int&gt;' requested here
   14 |     func_1(123);
      |     ^
&lt;source&gt;:6:9: note: candidate template ignored: couldn't infer template argument 'auto:1'
    6 |         [](auto)
      |         ^
1 error generated.
```

From my experimentation:
- `func_1` must be a function template;
- The outer lambda must be immediate;
- The outer lambda must be either polymorphic or a lambda template;
- The inner lambda must be either polymorphic or a lambda template.

Note: the code from which I wrote this test-case is almost the same where I originally found #<!-- -->117676, so there could be some relation (or not).
</details>


---

### Comment 2 - shafik

clang-19 regression: https://godbolt.org/z/hW5a1aqeW

CC @cor3ntin 

---

### Comment 3 - shafik

@katzdm git bisect points to this commit as to the cause of this regression: 5e767bd7d16dcdfc1ad8b32ba399f969dd940f57

---

### Comment 4 - katzdm

@shafik Sorry for ignoring this for a while - The changes from my commit looks to have been fully rolled back by a [recent change](https://github.com/llvm/llvm-project/pull/146857) by @cor3ntin , but the bug remains. I have some time this week, so I'll dig a bit to try to understand it.

---

### Comment 5 - katzdm

> [@shafik](https://github.com/shafik) Sorry for ignoring this for a while - The changes from my commit looks to have been fully rolled back by a [recent change](https://github.com/llvm/llvm-project/pull/146857) by [@cor3ntin](https://github.com/cor3ntin) , but the bug remains. I have some time this week, so I'll dig a bit to try to understand it.

Nevermind; I see that the logic I had added is now encapsulated by `PushExpressionEvaluationContextForFunction`.

---

