# Alias template CTAD is erroneously allowed in C++17 mode

**Author:** PJBoy
**URL:** https://github.com/llvm/llvm-project/issues/125913
**Status:** Closed
**Labels:** good first issue, c++17, clang:frontend, accepts-invalid, confirmed
**Closed Date:** 2025-04-03T13:58:54Z

## Body

Alias template CTAD is a C++20 feature ( https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L ) recently introduced in clang v19. Seems it's being allowed in C++17 mode with no diagnostic ( https://godbolt.org/z/x7zdxnrqx ). Though a (almost correct) diagnostic is issued with the `-Wpre-c++17-compat` flag ( https://godbolt.org/z/acKsG3dfj ).

Testcase is replicated here
```
template<typename T, typename TT>
struct S
{
    T t;
    TT tt;
};

template<typename T, typename TT>
S(T, TT) -> S<T, TT>;

template<typename T>
using S_int = S<T, int>;

int main()
{
    (void) S_int{1, 2};
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-17

Author: Patrick Johnston (PJBoy)

<details>
Alias template CTAD is a C++20 feature ( https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L ) recently introduced in clang v19. Seems it's being allowed in C++17 mode with no diagnostic ( https://godbolt.org/z/x7zdxnrqx ). Though a (almost correct) diagnostic is issued with the `-Wpre-c++17-compat` flag ( https://godbolt.org/z/acKsG3dfj ).

Testcase is replicated here
```
template&lt;typename T, typename TT&gt;
struct S
{
    T t;
    TT tt;
};

template&lt;typename T, typename TT&gt;
S(T, TT) -&gt; S&lt;T, TT&gt;;

template&lt;typename T&gt;
using S_int = S&lt;T, int&gt;;

int main()
{
    (void) S_int{1, 2};
}
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Patrick Johnston (PJBoy)

<details>
Alias template CTAD is a C++20 feature ( https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L ) recently introduced in clang v19. Seems it's being allowed in C++17 mode with no diagnostic ( https://godbolt.org/z/x7zdxnrqx ). Though a (almost correct) diagnostic is issued with the `-Wpre-c++17-compat` flag ( https://godbolt.org/z/acKsG3dfj ).

Testcase is replicated here
```
template&lt;typename T, typename TT&gt;
struct S
{
    T t;
    TT tt;
};

template&lt;typename T, typename TT&gt;
S(T, TT) -&gt; S&lt;T, TT&gt;;

template&lt;typename T&gt;
using S_int = S&lt;T, int&gt;;

int main()
{
    (void) S_int{1, 2};
}
```
</details>


---

### Comment 3 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 4 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Patrick Johnston (PJBoy)

<details>
Alias template CTAD is a C++20 feature ( https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L ) recently introduced in clang v19. Seems it's being allowed in C++17 mode with no diagnostic ( https://godbolt.org/z/x7zdxnrqx ). Though a (almost correct) diagnostic is issued with the `-Wpre-c++17-compat` flag ( https://godbolt.org/z/acKsG3dfj ).

Testcase is replicated here
```
template&lt;typename T, typename TT&gt;
struct S
{
    T t;
    TT tt;
};

template&lt;typename T, typename TT&gt;
S(T, TT) -&gt; S&lt;T, TT&gt;;

template&lt;typename T&gt;
using S_int = S&lt;T, int&gt;;

int main()
{
    (void) S_int{1, 2};
}
```
</details>


---

### Comment 5 - GeorgeKA

Hi @cor3ntin & @PJBoy  . I'm interested in working on this. Can it be assigned to me?

---

### Comment 6 - vinay-deshmukh

I'd like to pick this one up, if ok with you @GeorgeKA 

Haven't worked on the clang-frontend yet, so would appreciate any pointers on where to start looking at this issue. @cor3ntin 

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-c-17

Author: Patrick Johnston (PJBoy)

<details>
Alias template CTAD is a C++20 feature ( https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L ) recently introduced in clang v19. Seems it's being allowed in C++17 mode with no diagnostic ( https://godbolt.org/z/x7zdxnrqx ). Though a (almost correct) diagnostic is issued with the `-Wpre-c++17-compat` flag ( https://godbolt.org/z/acKsG3dfj ).

Testcase is replicated here
```
template&lt;typename T, typename TT&gt;
struct S
{
    T t;
    TT tt;
};

template&lt;typename T, typename TT&gt;
S(T, TT) -&gt; S&lt;T, TT&gt;;

template&lt;typename T&gt;
using S_int = S&lt;T, int&gt;;

int main()
{
    (void) S_int{1, 2};
}
```
</details>


---

### Comment 8 - GeorgeKA

Hi @vinay-deshmukh . Still working on this issue. (Had to wrap up another item.)

---

### Comment 9 - GeorgeKA

I don't think that this is actually a bug.

In the example given, you provided an explicit "_deduction guide_", which allows for successful CTAD in C++17, given C++17 can't do implicit deduction on its own. 
For clarity, your deduction guide: 
```
template<typename T, typename TT>
S(T, TT) -> S<T, TT>;
```
From my reading of the C++17 feature (see "[Deduction for alias templates](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction#Deduction%20for%20alias%20templates:~:text=Deduction%20for%20alias%20templates)") the deduction guide is being properly used here. "_Deduction will proceed in the same way as for class templates, except that the guides are instead generated from the guides of B._"


Note that without the explicit deduction guide, the sample code fails to compile in C++17, but successfully compiles in C++20.

C++17 with explicit deduction guide &nbsp;&nbsp;&emsp;- ✅: https://godbolt.org/z/q1eebMG3W
C++17 without explicit deduction guide - ❌: https://godbolt.org/z/b8v6M6rrc
C++20 without explicit deduction guide - ✅: https://godbolt.org/z/TxPYnTj17

As for the "almost correct diagnostic" (https://godbolt.org/z/acKsG3dfj), I think that's okay too. You used the _-Wpre-c++17-compat_ flag, i.e. before C++17 and CTAD being available at all. It reads:
```
warning: class template argument deduction is incompatible with C++ standards before C++17; for compatibility, use explicit type name 'S<int, int>' [-Wpre-c++17-compat]
```

Let me know what you think @PJBoy. 


---

### Comment 10 - MagentaTreehouse

> From my reading of the C++17 feature (see "[Deduction for alias templates](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction#Deduction%20for%20alias%20templates:~:text=Deduction%20for%20alias%20templates)") the deduction guide is being properly used here.

Deduction for alias templates is not a C++17 feature. Notice that the section in the link is marked "(since C++20)".

> Note that without the explicit deduction guide, the sample code fails to compile in C++17, but successfully compiles in C++20.

This is because it requires deduction for aggregates, also a C++20 feature.

@GeorgeKA 

---

### Comment 11 - GeorgeKA

@PJBoy I uploaded a patch but seems like the standard that we follow is that new features are allowed in old versions when it doesn't cause any problems. See [this comment from the PR](https://github.com/llvm/llvm-project/pull/133597#pullrequestreview-2727725511).

Is there any motivation for this other than the documented supported C++ version?
@MagentaTreehouse 

---

