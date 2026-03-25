# [clang] Linker regression with lambda NTTP containing `consteval` function call

**Author:** k3DW
**URL:** https://github.com/llvm/llvm-project/issues/169139
**Status:** Open
**Labels:** clang:codegen, miscompilation, consteval, regression:21

## Body

This following code worked in Clang 20, but no longer works in Clang 21. It also works in MSVC and GCC. I'm wondering whether this is a regression. https://godbolt.org/z/aYTTorW3o
```cpp
consteval int foo() {
    return 0;
}
template <auto fn>
void bar() {
    fn();
};
int main() {
    bar<[] { return foo(); }>();
}
```

The compile step succeeds, but the linking fails.

```none
ASM generation compiler returned: 0
/opt/compiler-explorer/gcc-15.2.0/lib/gcc/x86_64-linux-gnu/15.2.0/../../../../x86_64-linux-gnu/bin/ld: /tmp/example-a45c6a.o: in function `main::$_0::operator()() const':
<source>:9:(.text+0x3d): undefined reference to `foo()'
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
Execution build compiler returned: 1
Build failed
```

Note that this code works fine when the lambda is not a temporary. https://godbolt.org/z/6xPPczena
```cpp
// etc
int main() {
    constexpr auto x = [] { return foo(); };
    bar<x>();
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Braden Ganetsky (k3DW)

<details>
This following code worked in Clang 20, but no longer works in Clang 21. It also works in MSVC and GCC. I'm wondering whether this is a regression. https://godbolt.org/z/aYTTorW3o
```cpp
consteval int foo() {
    return 0;
}
template &lt;auto fn&gt;
void bar() {
    fn();
};
int main() {
    bar&lt;[] { return foo(); }&gt;();
}
```

The compile step succeeds, but the linking fails.

```none
ASM generation compiler returned: 0
/opt/compiler-explorer/gcc-15.2.0/lib/gcc/x86_64-linux-gnu/15.2.0/../../../../x86_64-linux-gnu/bin/ld: /tmp/example-a45c6a.o: in function `main::$_0::operator()() const':
&lt;source&gt;:9:(.text+0x3d): undefined reference to `foo()'
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
Execution build compiler returned: 1
Build failed
```

Note that this code works fine when the lambda is not a temporary. https://godbolt.org/z/6xPPczena
```cpp
// etc
int main() {
    constexpr auto x = [] { return foo(); };
    bar&lt;x&gt;();
}
```
</details>


---

### Comment 2 - k3DW

I found that the issue was introduced with [this commit](https://github.com/llvm/llvm-project/commit/5c1db3823e0d7ac820c938ca81adbffda3d98219). I don't know what change is needed however

---

### Comment 3 - shafik

CC @cor3ntin 

---

