# [clang] crash infinite recursion template template parameter default argument

**Author:** mizvekov
**URL:** https://github.com/llvm/llvm-project/issues/142842

## Body

Example: https://godbolt.org/z/5eKPfxK85
```C++
template<class T, template<class> class TT>     struct A;
template<class T, template<class> class TT = A> struct A {};
A<int> a;
```

This exhausts the stack before we hit the instantiation depth implementation limit:
```
<source>:2:46: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
    2 | template<class T, template<class> class TT = A> struct A {};
      |                                              ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matheus Izvekov (mizvekov)

<details>
Example: https://godbolt.org/z/5eKPfxK85
```C++
template&lt;class T, template&lt;class&gt; class TT&gt;     struct A;
template&lt;class T, template&lt;class&gt; class TT = A&gt; struct A {};
A&lt;int&gt; a;
```

This exhausts the stack before we hit the instantiation depth implementation limit:
```
&lt;source&gt;:2:46: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
    2 | template&lt;class T, template&lt;class&gt; class TT = A&gt; struct A {};
      |                                              ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Matheus Izvekov (mizvekov)

<details>
Example: https://godbolt.org/z/5eKPfxK85
```C++
template&lt;class T, template&lt;class&gt; class TT&gt;     struct A;
template&lt;class T, template&lt;class&gt; class TT = A&gt; struct A {};
A&lt;int&gt; a;
```

This exhausts the stack before we hit the instantiation depth implementation limit:
```
&lt;source&gt;:2:46: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
    2 | template&lt;class T, template&lt;class&gt; class TT = A&gt; struct A {};
      |                                              ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 3 - shafik

Goes back to clang-19: https://godbolt.org/z/1E9sdh6M9

---

### Comment 4 - mizvekov

It goes all the way back to clang-6: https://godbolt.org/z/Wh7v7aYGE
Before clang-19, you needed -frelaxed-template-template-args in order for the code to be accepted.
In clang-19 we made that flag the default.

---

