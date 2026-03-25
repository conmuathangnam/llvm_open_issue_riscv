# Failure to deduce nested template arguments

**Author:** Caslyn
**URL:** https://github.com/llvm/llvm-project/issues/140773
**Status:** Closed
**Labels:** clang:frontend, needs-reduction
**Closed Date:** 2025-05-22T06:05:53Z

## Body

After https://github.com/llvm/llvm-project/pull/138122, clang++ is failing to deduce a template parameter when instantiating a templated class object inside the ctor of another templated class object. In this case, we make an assignment to a `static constexpr` typed variable (ie we are not initializing a `static inline auto` variable).

@dty2 - could you help take a look into this and help determine whether this is an unintended consequence of  https://github.com/llvm/llvm-project/pull/138122?

Attached is a reproducer that emulates this new compilation failure:
[startup-random-867b56.zip](https://github.com/user-attachments/files/20353121/startup-random-867b56.zip)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Caslyn Tonelli (Caslyn)

<details>
After https://github.com/llvm/llvm-project/pull/138122, clang++ is failing to deduce a template parameter when instantiating a templated class object inside the ctor of another templated class object. In this case, we make an assignment to a `static constexpr` typed variable (ie we are not initializing a `static inline auto` variable).

@<!-- -->dty2 - could you help take a look into this and help determine whether this is an unintended consequence of  https://github.com/llvm/llvm-project/pull/138122?

Attached is a reproducer that emulates this new compilation failure:
[startup-random-867b56.zip](https://github.com/user-attachments/files/20353121/startup-random-867b56.zip)

</details>


---

### Comment 2 - ilovepi

After some digging, we're fairly certain that this is a regression. If not, we'll need a someone to explain exactly why this code is not correct, and that needs to be documented somewhere, since this is a non-obvious change in the compiler's behavior.

I'm trying to reduce the test case now to make the discussion and potential fixes easier to discuss.

A full bug report, with the error message and more analysis can be found here: https://fxbug.dev/418787016



---

### Comment 3 - cor3ntin

> After some digging, we're fairly certain that this is a regression. If not, we'll need a someone to explain exactly why this code is not correct, and that needs to be documented somewhere, since this is a non-obvious change in the compiler's behavior.
> 
> I'm trying to reduce the test case now to make the discussion and potential fixes easier to discuss.

This would certainly be very useful, thank you!

---

### Comment 4 - ilovepi

Well, I've held creduce poorly twice now and ended up with rather useless reductions. I have another running, but it may take a while .

---

### Comment 5 - AaronBallman

> After some digging, we're fairly certain that this is a regression. If not, we'll need a someone to explain exactly why this code is not correct, and that needs to be documented somewhere, since this is a non-obvious change in the compiler's behavior.

"fairly certain" is not "certain"; for example, changing template instantiation order can be a breaking change but still a conforming one that's necessary for correctness. No clue if that's what is happening in this situation, though. Having the reduced reproducer makes it easier for us to determine whether the behavioral change was intentional or not due to there being less moving parts to worry about.

> I'm trying to reduce the test case now to make the discussion and potential fixes easier to discuss.

Thank you!

> A full bug report, with the error message and more analysis can be found here: https://fxbug.dev/418787016

Not super relevant, but I got a kick out of "If there is some C++ language violation in what the code is doing, someone will have to explain it to me. Unless that is immediately forthcoming, the compiler change should be reverted." because that's not how things work. Hopefully it's understood that we make changes which intentionally break code all the time and that's not a cause for a revert. It's certainly a red flag when things used to compile (and still do in another compiler) but no longer do so, though, which is why we appreciate the issue and your help with getting us a reduced reproducer!

I would recommend that if you're not able to successfully reduce the issue in the next 24hrs, we should probably revert just to get back to the former status quo and can do the investigation with less pressure on everyone. Does that work?

---

### Comment 6 - ilovepi

```cpp
// RUN:  clang++ -cc1 -triple aarch64-unknown-fuchsia -emit-obj -disable-free  -target-cpu generic   -std=c++20  -x c++ %s

class a {
  public:
    using b = int;
};
enum c { cv };
# 63 "../../src/lib/elfldltl/include/lib/elfldltl/internal/const-string.h"
template <int> class ConstVector {

    void g() {}
};

template <class d> ConstVector(d) -> ConstVector<d().de>;

template <class e> class ConstString : e {

    template <class d>
    consteval ConstString(d) : ConstString{ConstVector{[] {}}} {}
};
template <class d> ConstString(d h) -> ConstString<typename decltype(h())::b>;
template <c = cv> struct i {
    template <char>
    static constexpr ConstString kFilename{[] {
        a dl;
        return dl;
    }};
};
template <class dk = i<>> class j {
    dk dn;
};
j<> f() {}
```
Godbolt: https://godbolt.org/z/Kx313ezc1

This is from creduce on the reproducer... I'm not 100% certain there isn't some key piece of template madness in the original that's been lost, but it compiles w/ clang 20, GCC15, and fails at ToT.

---

### Comment 7 - zyn0217

I simplified it further:

https://godbolt.org/z/bffEK1Gcb

```cpp
template <class Type> class ConstString : Type {
  template <class d>
  consteval ConstString(d) {}
};

template <class Func> ConstString(Func h)
  -> ConstString<typename decltype(h())::invalid>;

template <class = int>
struct Foo {
  template <char>
  static constexpr ConstString kFilename{[] {
    return 42;
  }};
};

Foo<> foo;
```

(@ilovepi Next time please avoid running rm-tokens/some other related passes in creduce; they make the reduction harder to decipher)

---

### Comment 8 - ilovepi

> Next time please avoid running rm-tokens/some other related passes in creduce; they make the reduction harder to decipher

Thanks for the tip. I'm far from an expert at creduce. 

---

