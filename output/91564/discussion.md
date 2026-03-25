# [clang][C++] Bad error recovery when classes are defined inside template aliases

**Author:** mizvekov
**URL:** https://github.com/llvm/llvm-project/issues/91564

## Body

When classes are defined inside template aliases, they are currently recovered as non templated classes, as if they were defined in the template alias' context. However, these classes let escape any references to the template parameters.

Consider this example:
```C++
template <class T> using A = struct B {
  template <class> void f() requires (T()); // BAD: T aliases f's unnamed template parameter.
};
template void B::f<void>();
```

Produces:
```
test.cc:1:37: error: 'B' cannot be defined in a type alias template
    1 | template <class T> using A = struct B {
      |                                     ^
test.cc:2:39: error: atomic constraint must be of type 'bool' (found 'void')
    2 |   template <class> void f() requires (T());
```

This can easily lead to crashes as well:
```C++
template <class T> using A = struct B {
  using C = T;
};
template <class> void f() requires (B::C());
template void f<void>();
```

asserts in the constexpr evaluator:
```
Assertion failed: (!isValueDependent() && "Expression evaluator can't be called on a dependent expression."), function EvaluateAsConstantExpr, file ExprConstant.cpp, line 15953.
```

---

Probably a good strategy to fix this issue is to mark the alias' template parameters as invalid, and make sure their resulting type is something sensible for error recovery.

## Comments

### Comment 1 - llvmbot


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

### Comment 2 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Matheus Izvekov (mizvekov)

<details>
When classes are defined inside template aliases, they are currently recovered as non templated classes, as if they were defined in the template alias' context. However, these classes let escape any references to the template parameters.

Consider this example:
```C++
template &lt;class T&gt; using A = struct B {
  template &lt;class&gt; void f() requires (T()); // BAD: T aliases f's unnamed template parameter.
};
template void B::f&lt;void&gt;();
```

Produces:
```
test.cc:1:37: error: 'B' cannot be defined in a type alias template
    1 | template &lt;class T&gt; using A = struct B {
      |                                     ^
test.cc:2:39: error: atomic constraint must be of type 'bool' (found 'void')
    2 |   template &lt;class&gt; void f() requires (T());
```

This can easily lead to crashes as well:
```C++
template &lt;class T&gt; using A = struct B {
  using C = T;
};
template &lt;class&gt; void f() requires (B::C());
template void f&lt;void&gt;();
```

asserts in the constexpr evaluator:
```
Assertion failed: (!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."), function EvaluateAsConstantExpr, file ExprConstant.cpp, line 15953.
```

---

Probably a good strategy to fix this issue is to mark the alias' template parameters as invalid, and make sure their resulting type is something sensible for error recovery.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matheus Izvekov (mizvekov)

<details>
When classes are defined inside template aliases, they are currently recovered as non templated classes, as if they were defined in the template alias' context. However, these classes let escape any references to the template parameters.

Consider this example:
```C++
template &lt;class T&gt; using A = struct B {
  template &lt;class&gt; void f() requires (T()); // BAD: T aliases f's unnamed template parameter.
};
template void B::f&lt;void&gt;();
```

Produces:
```
test.cc:1:37: error: 'B' cannot be defined in a type alias template
    1 | template &lt;class T&gt; using A = struct B {
      |                                     ^
test.cc:2:39: error: atomic constraint must be of type 'bool' (found 'void')
    2 |   template &lt;class&gt; void f() requires (T());
```

This can easily lead to crashes as well:
```C++
template &lt;class T&gt; using A = struct B {
  using C = T;
};
template &lt;class&gt; void f() requires (B::C());
template void f&lt;void&gt;();
```

asserts in the constexpr evaluator:
```
Assertion failed: (!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."), function EvaluateAsConstantExpr, file ExprConstant.cpp, line 15953.
```

---

Probably a good strategy to fix this issue is to mark the alias' template parameters as invalid, and make sure their resulting type is something sensible for error recovery.
</details>


---

### Comment 4 - changkhothuychung

Can I take this issue if its available? thanks. 

---

### Comment 5 - mizvekov

Yeah go ahead.

---

### Comment 6 - changkhothuychung

@mizvekov M
Hi, thanks for much, can you assign the issue to me? Also, this is my first time contributing to clang:frontend, can you give me some instructions what file/function I should modify? 

---

### Comment 7 - mizvekov

> @mizvekov M
> 
> Hi, thanks for much, can you assign the issue to me?

I believe you should be able to just assign yourself, what happens if you click that on the right?

> Also, this is my first time contributing to clang:frontend, can you give me some instructions what file/function I should modify? 

I think your first order of business should be to locate where the first diagnostic is produced. You should be able to grep the english text, find the tablegen entry for it in a .td file, and then grep for it's identifier in the source code.


---

### Comment 8 - rajkumarananthu

Hi Team, is this issue still open for contribution..  I would like to work on this. I don't have control to assign the issue myself.. so can someone assign this to me if it is open for contribution.

Thanks
Rajkumar Ananthu.

---

### Comment 9 - shafik

> Hi Team, is this issue still open for contribution.. I would like to work on this. I don't have control to assign the issue myself.. so can someone assign this to me if it is open for contribution.
> 
> Thanks Rajkumar Ananthu.

Done

---

### Comment 10 - firstmoonlight

Hi, @rajkumarananthu, Are you still working on this issue? I found it has being opening for more than one month. If not, I'd like to working on this issue.

---

### Comment 11 - rajkumarananthu

Hi @firstmoonlight , my day job did not allow me to take a look at the issue, I am un-assigning myself from the issue, please feel free to  work on it, sorry for the delay in response.

Thanks
Rajkumar.

---

### Comment 12 - erichkeane

Unassigning, no movement on this/response back from @firstmoonlight, so opening so that anyone (including him!) can work on it ).

---

### Comment 13 - shivam-dhir

Hi, I would love to work on this issue

---

