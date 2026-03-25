# Clang++ rejects try-catch in discarded if constexpr branch with -fno-exceptions

**Author:** zerhud
**URL:** https://github.com/llvm/llvm-project/issues/138939
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, constexpr, diverges-from:gcc
**Closed Date:** 2025-05-28T14:10:56Z

## Body

**Summary**:
Clang incorrectly rejects code containing a `try-catch` block inside a discarded `if constexpr` branch when compiled with `-fno-exceptions`. The `try-catch` block should not be evaluated, as it is excluded at compile-time, but Clang emits an error: `cannot use 'try' with exceptions disabled`.

**Steps to Reproduce**:
1. Create a file `test.cpp` with the following code:
   ```cpp
   template<auto enable> void foo(auto&& fnc) {
       if constexpr(enable) try{ fnc(); } catch(...){}
       else fnc();
   }

   int main(int, char**) {
       foo<false>([]{});
   }
   ```
2. Compile with:
   ```bash
   clang++ -std=c++20 -fno-exceptions -c test.cpp
   ```
3. Alternatively, see the issue on [Godbolt](https://godbolt.org/z/3ecnde5z4): https://godbolt.org/z/3ecnde5z4

**Actual Results**:
```
test.cpp:2:25: error: cannot use 'try' with exceptions disabled
    if constexpr(enable) try{ fnc(); } catch(...){}
                         ^
1 error generated.
```

**Expected Results**:
The code should compile successfully, as the `try-catch` block is in a discarded `if constexpr` branch and should not be evaluated.

**Additional Information**:
- The code compiles successfully with GCC
- A not called template function with try-catch block rejected too ([Godbolt](https://godbolt.org/z/4nb3vcYTo))

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (zerhud)

<details>
**Summary**:
Clang incorrectly rejects code containing a `try-catch` block inside a discarded `if constexpr` branch when compiled with `-fno-exceptions`. The `try-catch` block should not be evaluated, as it is excluded at compile-time, but Clang emits an error: `cannot use 'try' with exceptions disabled`.

**Steps to Reproduce**:
1. Create a file `test.cpp` with the following code:
   ```cpp
   template&lt;auto enable&gt; void foo(auto&amp;&amp; fnc) {
       if constexpr(enable) try{ fnc(); } catch(...){}
       else fnc();
   }

   int main(int, char**) {
       foo&lt;false&gt;([]{});
   }
   ```
2. Compile with:
   ```bash
   clang++ -std=c++20 -fno-exceptions -c test.cpp
   ```
3. Alternatively, see the issue on [Godbolt](https://godbolt.org/z/3ecnde5z4): https://godbolt.org/z/3ecnde5z4

**Actual Results**:
```
test.cpp:2:25: error: cannot use 'try' with exceptions disabled
    if constexpr(enable) try{ fnc(); } catch(...){}
                         ^
1 error generated.
```

**Expected Results**:
The code should compile successfully, as the `try-catch` block is in a discarded `if constexpr` branch and should not be evaluated.

**Additional Information**:
- The code compiles successfully with GCC
- A not called template function with try-catch block rejected too ([Godbolt](https://godbolt.org/z/4nb3vcYTo))
</details>


---

### Comment 2 - shafik

Looks like other implementations accept w/ their various no exceptions flags: https://godbolt.org/z/YhqaxTW3E

---

### Comment 3 - erichkeane

Technically not a bug/not incorrect, since we define what that flag means as it is non standard:)  That said, all of the other implementations seem to diagnose this 'later' than we do so they accept it, and we should too.

I also note we don't document what `-fno-exceptions` means, though we probably should!

As far as implementation: We currently diagnose this in `ActOnCXXTryBlock` and `BuildCXXThrow`.
Those are propbably the correct places here, but what we need to do is change that to ONLY diagnose when it is in a non-dependent context.

Additionally, we'd want to make sure that this doesn't cause us to lose diagnostics in cases like:
```
template<typename T>
void foo() {
   try{}catch(...){}
   throw 1;
}
void use() {
  foo<int>();
}
```
If we DO miss those two (which again should only happen because of `foo<int>()`, so should print the instantiation notes) we might have to move one/both of the diagnostics.

I think I'm going to mark this as a good beginner issue.  It might be slightly non-trivial, and if it ends up being, whoever decides to work on it can feel free to reach out to me for advice.


---

### Comment 4 - llvmbot


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

### Comment 5 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (zerhud)

<details>
**Summary**:
Clang incorrectly rejects code containing a `try-catch` block inside a discarded `if constexpr` branch when compiled with `-fno-exceptions`. The `try-catch` block should not be evaluated, as it is excluded at compile-time, but Clang emits an error: `cannot use 'try' with exceptions disabled`.

**Steps to Reproduce**:
1. Create a file `test.cpp` with the following code:
   ```cpp
   template&lt;auto enable&gt; void foo(auto&amp;&amp; fnc) {
       if constexpr(enable) try{ fnc(); } catch(...){}
       else fnc();
   }

   int main(int, char**) {
       foo&lt;false&gt;([]{});
   }
   ```
2. Compile with:
   ```bash
   clang++ -std=c++20 -fno-exceptions -c test.cpp
   ```
3. Alternatively, see the issue on [Godbolt](https://godbolt.org/z/3ecnde5z4): https://godbolt.org/z/3ecnde5z4

**Actual Results**:
```
test.cpp:2:25: error: cannot use 'try' with exceptions disabled
    if constexpr(enable) try{ fnc(); } catch(...){}
                         ^
1 error generated.
```

**Expected Results**:
The code should compile successfully, as the `try-catch` block is in a discarded `if constexpr` branch and should not be evaluated.

**Additional Information**:
- The code compiles successfully with GCC
- A not called template function with try-catch block rejected too ([Godbolt](https://godbolt.org/z/4nb3vcYTo))
</details>


---

### Comment 6 - GeorgeKA

Hi @zerhud, @erichkeane. Could this be assigned to me?

---

### Comment 7 - erichkeane

> Hi [@zerhud](https://github.com/zerhud), [@erichkeane](https://github.com/erichkeane). Could this be assigned to me?

We are trying out a strategy where we don't actually do the assignment (as that discourages folks from looking at this in the future if you don't end up completing it), but count on the comments to let people judge whether it is available or not.  So your comment will suffice. 

---

### Comment 8 - zerhud

> Technically not a bug/not incorrect, since we define what that flag means as it is non standard:) 

Nice note!

> That said, all of the other implementations seem to diagnose this 'later' than we do so they accept it, and we should too.

may be one more reason. a library can allow user to customize their behavior by adding some functions to realization class. for example
```C++

namespace lib::customization {

template<typename factory> void customize_exceptions_for_case_x(auto&& fnc) {
    if constexpr(!requires{ factory::on_case_x_exception(); }) fnc();
    else try{ fnc(); } catch(...) { factory::on_case_x_exception(); }
}

}
```

such pattern allows user for example use `std::throw_with_nested` or use it right "out of the box", but it doesn't allow use it with `-fno-exceptions`. in that case user "pay" for something what are not used. it seems the only option here is use macros (e.g. `#ifdef __EXCEPTIONS`) in customization function.


> Additionally, we'd want to make sure that this doesn't cause us to lose diagnostics in cases like:
> 
> ```
> template<typename T>
> void foo() {
>    try{}catch(...){}
>    throw 1;
> }
> void use() {
>   foo<int>();
> }
> ```

if you are talking about `throw 1;` - the code clearly should not be available with `-fno-exceptions` option. but the code `try{}catch(...){}` do nothing and it seems can to be simple ignored. i believe something like that should to be discarded:

```C++
void bar();
template<typename> void foo() {
    try{ bar(); }catch(...){}
}
void use() {
    foo<int>();
}
```
because of
1. the try-catch block has a payload and cannot to be ignored
2. the `foo` was instantiated


---

### Comment 9 - erichkeane


> may be one more reason. a library can allow user to customize their behavior by adding some functions to realization class. for example
> 
> namespace lib::customization {
> 
> template<typename factory> void customize_exceptions_for_case_x(auto&& fnc) {
>     if constexpr(!requires{ factory::on_case_x_exception(); }) fnc();
>     else try{ fnc(); } catch(...) { factory::on_case_x_exception(); }
> }
> 
> }
> 
> such pattern allows user for example use `std::throw_with_nested` or use it right "out of the box", but it doesn't allow use it with `-fno-exceptions`. in that case user "pay" for something what are not used. it seems the only option here is use macros (e.g. `#ifdef __EXCEPTIONS`) in customization function.

The above, where the try/catch is in the discarded statement I think is completely justifiable and a pattern I'm Ok supporting.
> 
> > Additionally, we'd want to make sure that this doesn't cause us to lose diagnostics in cases like:
> > ```
> > template<typename T>
> > void foo() {
> >    try{}catch(...){}
> >    throw 1;
> > }
> > void use() {
> >   foo<int>();
> > }
> > ```
> 
> if you are talking about `throw 1;` - the code clearly should not be available with `-fno-exceptions` option. but the code `try{}catch(...){}` do nothing and it seems can to be simple ignored. i believe something like that should to be discarded:
> 

It is available by 'language', and we don't want to be in the business of trying to recognize what are 'real' try/catch statements and what aren't.  I would want us to diagnose both the try and the throw there.

> void bar();
> template<typename> void foo() {
>     try{ bar(); }catch(...){}
> }
> void use() {
>     foo<int>();
> }
> 
> because of
> 
>     1. the try-catch block has a payload and cannot to be ignored
> 
>     2. the `foo` was instantiated


Whether or not the `try` has statements we should diagnose it as soon as it is instantiated.

---

