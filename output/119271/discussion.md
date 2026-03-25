# clang fails deduction on array when adding const qualification

**Author:** brevzin
**URL:** https://github.com/llvm/llvm-project/issues/119271

## Body

To be honest, I'm not entirely sure that this is a clang bug (and @zygoloid can put me in my place quickly enough).

Here's an interesting example:

```cpp
using size_t = decltype(sizeof(0));

template <size_t N>
void f(const int* const (&)[N]);

template <class T>
void g(T const* const*);

int main() {
  int* pointers[3];
  f(pointers); // clang: error, gcc: ok

  g(pointers); // clang, gcc: ok

  const int* const (&h)[3] = pointers; // clang, gcc: ok
}
```

All three of these situations seem similar: we're taking an array of pointers to mutable `int` (no `const` anywhere) and trying to add `const` in two layers:

* `f` is trying to deduce a reference to an array of const pointers to const `int` (gcc is okay with this, clang fails deduction saying it cannot match `const int *` against `int*`)
* `g` is trying to deduce a pointer to const pointer to const `int` (which is similar, and both gcc and clang are fine with this)
* `h` is just directly binding the same reference that `f` would have, except not in a template deduction context (likewise similar, both gcc and clang are fine with this too)

At the very least, it seems like `f` and `h` should either both be valid or both be invalid. So I think it's either a clang bug (so I'm in the right spot) or a C++ bug (and I should be writing this issue in a different spot). 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Barry Revzin (brevzin)

<details>
To be honest, I'm not entirely sure that this is a clang bug (and @<!-- -->zygoloid can put me in my place quickly enough).

Here's an interesting example:

```cpp
using size_t = decltype(sizeof(0));

template &lt;size_t N&gt;
void f(const int* const (&amp;)[N]);

template &lt;class T&gt;
void g(T const* const*);

int main() {
  int* pointers[3];
  f(pointers); // clang: error, gcc: ok

  g(pointers); // clang, gcc: ok

  const int* const (&amp;h)[3] = pointers; // clang, gcc: ok
}
```

All three of these situations seem similar: we're taking an array of pointers to mutable `int` (no `const` anywhere) and trying to add `const` in two layers:

* `f` is trying to deduce a reference to an array of const pointers to const `int` (gcc is okay with this, clang fails deduction saying it cannot match `const int *` against `int*`)
* `g` is trying to deduce a pointer to const pointer to const `int` (which is similar, and both gcc and clang are fine with this)
* `h` is just directly binding the same reference that `f` would have, except not in a template deduction context (likewise similar, both gcc and clang are fine with this too)

At the very least, it seems like `f` and `h` should either both be valid or both be invalid. So I think it's either a clang bug (so I'm in the right spot) or a C++ bug (and I should be writing this issue in a different spot). 
</details>


---

### Comment 2 - zygoloid

EDG and MSVC agree with Clang that the call to `f` is ill-formed. MSVC thinks the reference binding that we're calling `h` is ill-formed too. Clang accepts if the template argument is explicit specified: `f<3>(pointers)`, whereas MSVC and EDG still reject in that case. So all implementations disagree with each other.

I think there are two things at play here:

1) Performing *implicit conversion* from `pointers` to `const int* const (&)[3]` isn't the same thing as forming an *implicit conversion sequence* from `pointers` to `const int* const (&)[3]`. The former (which is equivalent to copy-initialization, defined in [dcl.init]) is what happens during initialization, and the latter (defined in [over.best.ics]) is the theoretical question of "what do we think initialization would do?" that overload resolution uses.
2) Until recently, implicit conversion sequence formation and overload resolution were *not* performed if there was only a single candidate function. This rule changed and the new rule is that you always do overload resolution.

So, I think what's going on here is:

- Clang thinks that the implicit conversion is valid, but the implicit conversion sequence is not. Clang does not do overload resolution when it has only one candidate, so fails to notice that `f<3>(pointers)` forms an invalid implicit conversion sequence.
- EDG, like Clang, thinks that the implicit conversion is valid but the implicit conversion sequence is not. And it checks for an implicit conversion sequence for `f<3>(pointers)`. (But it doesn't do so for a call to a non-template function -- it appears that EDG literally implements the old rule, which is that you do overload resolution if you have more than one function or any function template in the overload set, whereas Clang also skips it when there's a single function template specialization.)
- GCC thinks the implicit conversion and implicit conversion sequence are both valid.
- MSVC thinks both are invalid.

OK. So what's the actual disposition of the example?

For implicit conversion:

- After CWG2352, the reference type is reference-compatible with the type of `pointers`, so this is valid and the reference binds directly to `pointers`.
- Prior to CWG2352, the reference would have tried to bind to a temporary of type `const int*[3]`. But that reference binding would try to copy-initialize a `const int *temp[3]` from `pointers`, and such copy-initialization is not valid. So prior to CWG2352, the initialization was ill-formed (it would try and fail to form a temporary of array type).

Therefore accepting the reference binding is correct; everyone but MSVC gets this right.

For the implicit conversion sequence:

- After CWG2352, the reference binds directly to `pointers`, so this should be valid per [over.ics.ref]/1, which delegates to [dcl.init.ref] to determine if the reference would bind directly. But CWG2352 didn't explicitly call out any effect on implicit conversion sequence formation, so perhaps that's why Clang and EDG both missed this.
- Prior to CWG2352, [over.ics.ref]/2 says we consider an implicit conversion sequence from array of `int*` to array of `const int*`. That appears to technically be possible by an lvalue-to-rvalue conversion followed by a qualification conversion, but I'm pretty sure it's not intended to be possible! So this was valid before CWG2352 too (though shouldn't have been).

Looks to me that:

- GCC is right.
- Clang's implicit conversion sequence behavior is wrong.
- EDG's implicit conversion sequence behavior is wrong.
- MSVC's implicit conversion and implicit conversion sequence behavior is wrong.

And various implementations (maybe all of them) don't properly implement the rule that we always perform overload resolution, even for non-overloaded functions.

---

### Comment 3 - brevzin

There are days when I think I know this language. And then I ask you a question and read your answer...

Thank you!

---

### Comment 4 - t3nsor

[CWG2803](https://cplusplus.github.io/CWG/issues/2803.html) adds specification for implicit conversion sequences that was omitted from CWG2352.

I believe there is still a bug in the wording. [[temp.deduct.call]/4](https://eel.is/c++draft/temp.deduct.call#4) doesn't care whether the reference binding was direct or not, but
* bullet 4.1 can be read as allowing the addition of top-level cv-qualification only
* bullet 4.2 requires the argument type to be a pointer, which it isn't

So the example falls into a gap in the template deduction wording.

---

### Comment 5 - zygoloid

> So the example falls into a gap in the template deduction wording.

Ah right, and indeed both EDG and Clang are rejecting the call to `f` due to a deduction failure, presumably for exactly this reason. It looks like EDG still does template argument deduction for `f<3>(pointers)` and then rejects because of a mismatch in the first parameter's type (even though there's nothing to deduce in the first parameter after substituting `N = 3`); it accepts `(*f<3>)(pointers)`.

---

### Comment 6 - t3nsor

I've requested a CWG issue: https://github.com/cplusplus/CWG/issues/651

---

