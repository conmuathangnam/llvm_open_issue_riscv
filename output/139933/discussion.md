# Thread safety/capability analysis behavioral difference between member and free function

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/139933
**Status:** Closed
**Labels:** wontfix, clang:frontend, clang:analysis
**Closed Date:** 2025-05-22T12:47:42Z

## Body

The following code emits unexpected diagnostics about the capability expecting to be held or still being held within the `*_permission` functions:
```
#if __has_cpp_attribute(clang::capability)
#define CAPABILITY(Name) [[clang::capability(Name)]]
#define ACQUIRE(Cap) [[clang::acquire_capability(Cap)]]
#define RELEASE(Cap) [[clang::release_capability(Cap)]]
#define REQUIRE(Cap) [[clang::requires_capability(Cap)]]
#else
#define CAPABILITY(Name)
#define ACQUIRE(Cap)
#define RELEASE(Cap)
#define REQUIRE(Cap)
#endif

struct CAPABILITY("special permission") SpecialPerms {
} SpecialPermissions;

ACQUIRE(SpecialPermissions) void give_me_permission() {}
RELEASE(SpecialPermissions) void revoke_my_permission() {}

REQUIRE(SpecialPermissions) void func() {}

void good_call() {
  give_me_permission();
  func();
  revoke_my_permission();
}
```
https://godbolt.org/z/d8Ezhsn88

But with a tweak to using member functions instead, no diagnostics are issued:
```
#if __has_cpp_attribute(clang::capability)
#define CAPABILITY(Name) [[clang::capability(Name)]]
#define ACQUIRE(Cap) [[clang::acquire_capability(Cap)]]
#define RELEASE(Cap) [[clang::release_capability(Cap)]]
#define REQUIRE(Cap) [[clang::requires_capability(Cap)]]
#else
#define CAPABILITY(Name)
#define ACQUIRE(Cap)
#define RELEASE(Cap)
#define REQUIRE(Cap)
#endif

struct CAPABILITY("special permission") SpecialPerms {
  ACQUIRE() void give_me_permission() {}
  RELEASE() void revoke_my_permission() {}
} SpecialPermissions;

REQUIRE(SpecialPermissions) void func() {}

void good_call() {
  SpecialPermissions.give_me_permission();
  func();
  SpecialPermissions.revoke_my_permission();
}
```
https://godbolt.org/z/Eoh8xKnje

Am I holding it wrong in the free function example or is this an issue with the analysis?

## Comments

### Comment 1 - AaronBallman

CC @aaronpuchert 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
The following code emits unexpected diagnostics about the capability expecting to be held or still being held within the `*_permission` functions:
```
#if __has_cpp_attribute(clang::capability)
#define CAPABILITY(Name) [[clang::capability(Name)]]
#define ACQUIRE(Cap) [[clang::acquire_capability(Cap)]]
#define RELEASE(Cap) [[clang::release_capability(Cap)]]
#define REQUIRE(Cap) [[clang::requires_capability(Cap)]]
#else
#define CAPABILITY(Name)
#define ACQUIRE(Cap)
#define RELEASE(Cap)
#define REQUIRE(Cap)
#endif

struct CAPABILITY("special permission") SpecialPerms {
} SpecialPermissions;

ACQUIRE(SpecialPermissions) void give_me_permission() {}
RELEASE(SpecialPermissions) void revoke_my_permission() {}

REQUIRE(SpecialPermissions) void func() {}

void good_call() {
  give_me_permission();
  func();
  revoke_my_permission();
}
```
https://godbolt.org/z/d8Ezhsn88

But with a tweak to using member functions instead, no diagnostics are issued:
```
#if __has_cpp_attribute(clang::capability)
#define CAPABILITY(Name) [[clang::capability(Name)]]
#define ACQUIRE(Cap) [[clang::acquire_capability(Cap)]]
#define RELEASE(Cap) [[clang::release_capability(Cap)]]
#define REQUIRE(Cap) [[clang::requires_capability(Cap)]]
#else
#define CAPABILITY(Name)
#define ACQUIRE(Cap)
#define RELEASE(Cap)
#define REQUIRE(Cap)
#endif

struct CAPABILITY("special permission") SpecialPerms {
  ACQUIRE() void give_me_permission() {}
  RELEASE() void revoke_my_permission() {}
} SpecialPermissions;

REQUIRE(SpecialPermissions) void func() {}

void good_call() {
  SpecialPermissions.give_me_permission();
  func();
  SpecialPermissions.revoke_my_permission();
}
```
https://godbolt.org/z/Eoh8xKnje

Am I holding it wrong in the free function example or is this an issue with the analysis?
</details>


---

### Comment 3 - aaronpuchert

We currently only except functions with no attribute arguments from the analysis:
```c++
      } else if (const auto *A = dyn_cast<ReleaseCapabilityAttr>(Attr)) {
        // UNLOCK_FUNCTION() is used to hide the underlying lock implementation.
        // We must ignore such methods.
        if (A->args_size() == 0)
          return;
        // ...
      } else if (const auto *A = dyn_cast<AcquireCapabilityAttr>(Attr)) {
        if (A->args_size() == 0)
          return;
        // ...
      } else if (isa<TryAcquireCapabilityAttr>(Attr)) {
        // Don't try to check trylock functions for now.
        return;
      }
```
This means member functions, where we have an implicit `this` argument. I think the problem with free functions is that they might or might not be part of the implementation of the capability. Member functions have to be listed in the class itself, so we can assume they're part of the implementation. (Interestingly, there was a request to remove even this exception in #56815.)

Workaround is to add `__attribute__((no_thread_safety_analysis))`.

---

### Comment 4 - AaronBallman

Hmmm, `no_thread_safety_analysis` is a viable workaround, but this is still pretty unfortunate because it makes using these interfaces from C really frustrating because C doesn't have member functions, only free functions. I think that whether something is part of the implementation of the capability is at least somewhat determined by the set of attributes involved, right? e.g., if something acquires or releases a capability, that's part of the capability interface, isn't it?

---

### Comment 5 - aaronpuchert

> C doesn't have member functions, only free functions.

Yes, and it doesn't need member functions because it doesn't have access rules. This makes it difficult to tell apart implementation and users. I could see us carving out the C equivalent to member functions, namely where the attribute argument is a `DeclRefExpr` of a `ParmVarDecl`, maybe even requiring that there be only one parameter or that it be the first. But this wouldn't help in your example, only in the classic mutex case. (Though in your example, a difference is that the member functions are non-static whereas the free functions have no arguments. So this is not equivalent.)

> I think that whether something is part of the implementation of the capability is at least somewhat determined by the set of attributes involved, right? e.g., if something acquires or releases a capability, that's part of the capability interface, isn't it?

Attributes with capability arguments are regularly used in "user code". So I think we'd carve out too much here.

Just out of curiousity, I disabled the analysis for functions with any acquire/release attributes and ran the [big test](https://github.com/llvm/llvm-project/blob/a7ede51b556f40163db9e3cc67c98c27ba2364d8/clang/test/SemaCXX/warn-thread-safety-analysis.cpp):
```
error: 'expected-warning' diagnostics expected but not seen:
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5169: expecting mutex 'mu_' to be held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5174: expecting mutex 'mu_' to be held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5179: mutex 'mu_' is still held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5184: expecting mutex 'mu_' to be held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5184: mutex 'mu2_' is still held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5190: expecting mutex 'mu_' to be held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5190: mutex 'mu2_' is still held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5195: releasing mutex 'mu2_' that was not held
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5196: mutex 'mu_' is still held at the end of function
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 6225: reading variable 'foo' requires holding mutex 'mu'
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 6247: returning variable 'foo' by reference requires holding mutex 'mu' exclusively
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 6252: returning variable 'foo' by reference requires holding mutex 'mu' exclusively
error: 'expected-note' diagnostics expected but not seen:
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5166: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5171: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5176: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5182: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5183: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5188: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5189: mutex acquired here
  File clang/test/SemaCXX/warn-thread-safety-analysis.cpp Line 5194: mutex acquired here
20 errors generated.
```
For example, the first missing warning:
```c++
  // Check failure to lock.
  void lockBad() EXCLUSIVE_LOCK_FUNCTION(mu_) {    // expected-note {{mutex acquired here}}
    mu2_.Lock();
    mu2_.Unlock();
  }  // expected-warning {{expecting mutex 'mu_' to be held at the end of function}}
```
This doesn't seem like an oversight.

If you think about it, part of the analysis is determining the initial and expected final lock set based on the attributes. If we'd disable the analysis when we have these attributes, we would never have a differing initial and final lock set.

---

### Comment 6 - AaronBallman

Thanks for the discussion! I think we can close this issue as it's behaving by design and there is a workaround with `no_thread_safety_analysis`. I still think that workaround is unsatisfying because it means you cannot use the capability attributes (without workarounds) for a pretty reasonable use case (IMO), but it's also not the end of the world.

---

### Comment 7 - aaronpuchert

Still, I posted an idea: #141432.

---

