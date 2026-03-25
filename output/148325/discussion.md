# Crash on 19.10, 20.0, and trunk

**Author:** SiliconA-Z
**URL:** https://github.com/llvm/llvm-project/issues/148325

## Body

https://godbolt.org/z/d5ha1EEj4

change the using WinAPIGLFunc = __attribute__((ms_abi)) GLFunc; to using WinAPIGLFunc = __attribute__((ms_abi)) Result (*)(Args...); and it doesnt crash

```
#include <stdint.h>
#include <utility>

template<class T>
class GLFunctor;

template<class Result, class... Args>
class GLFunctor<Result (Args...)>
{
	using GLFunc = Result (*)(Args...);

public:
	Result operator()(Args... args) const
	{
		using WinAPIGLFunc = __attribute__((ms_abi)) GLFunc;
		return reinterpret_cast<WinAPIGLFunc>(glProc)(std::forward<Args>(args)...);
	}
private:
	GLFunc glProc = nullptr;
};

GLFunctor<void (int pname)> test_functor;

void Test()
{
    test_functor(0);
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: AZero13 (AZero13)

<details>
https://godbolt.org/z/d5ha1EEj4

change the using WinAPIGLFunc = __attribute__((ms_abi)) GLFunc; to using WinAPIGLFunc = __attribute__((ms_abi)) Result (*)(Args...); and it doesnt crash

```
#include &lt;stdint.h&gt;
#include &lt;utility&gt;

template&lt;class T&gt;
class GLFunctor;

template&lt;class Result, class... Args&gt;
class GLFunctor&lt;Result (Args...)&gt;
{
	using GLFunc = Result (*)(Args...);

public:
	Result operator()(Args... args) const
	{
		using WinAPIGLFunc = __attribute__((ms_abi)) GLFunc;
		return reinterpret_cast&lt;WinAPIGLFunc&gt;(glProc)(std::forward&lt;Args&gt;(args)...);
	}
private:
	GLFunc glProc = nullptr;
};

GLFunctor&lt;void (int pname)&gt; test_functor;

void Test()
{
    test_functor(0);
}
```
</details>


---

### Comment 2 - shafik

Started in clang-19

---

### Comment 3 - shafik

@jcsxky git bisect points to d9d1ae6400a7f8a12068bdd37ecda62f07e52bce

---

### Comment 4 - cor3ntin

simplified a bit
https://godbolt.org/z/fxYh67dhM

```cpp
template <class T> class GLFunctor;

template <class... Args> 
struct GLFunctor<void(Args...)> {
    using GLFunc = void (*)(Args...);
    void operator()(Args... args) const {
        using WinAPIGLFunc = __attribute__((ms_abi)) GLFunc;
    }
};

void Test() { GLFunctor<void(int pname)>{}(0); }
```

---

