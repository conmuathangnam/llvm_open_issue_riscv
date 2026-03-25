# Wrongful cleanup for `trivial_abi` parameter after passing it to callee

**Author:** aaronpuchert
**URL:** https://github.com/llvm/llvm-project/issues/60112
**Status:** Closed
**Labels:** clang:codegen, miscompilation
**Closed Date:** 2025-04-14T15:58:34Z

## Body

Invoking `caller` in the following code leads to double destruction:

```c++
struct __attribute__((trivial_abi)) trivial {
  trivial() = default;
  trivial(const trivial &) noexcept;
  ~trivial();
  int* p = nullptr;
};

struct other {
  other() = default;
  other(const other&) noexcept;
  ~other();
};

void callee(trivial f, other) { throw 0; }

bool b = true;

void caller() {
  trivial f;
  other n;
  b ? callee(f, n) : void();
}
```

Since `trivial` has a `trivial_abi` attribute, it must generally be cleaned up by the callee (except if initialization of another parameter fails, which cannot happen here due to the `noexcept` on the `other` copy constructor). But the code around the call looks like this:

```llvm
define dso_local void @_Z6callerv() #0 personality ptr @__gxx_personality_v0 {
entry:
  ; ... setting up locals
  store i1 false, ptr %cleanup.cond, align 1
  store i1 false, ptr %cleanup.cond2, align 1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  call void @_ZN7trivialC1ERKS_(ptr noundef nonnull align 8 dereferenceable(8) %agg.tmp, ptr noundef nonnull align 8 dereferenceable(8) %f) #3
  store i1 true, ptr %cleanup.cond, align 1
  call void @_ZN5otherC1ERKS_(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1, ptr noundef nonnull align 1 dereferenceable(1) %n) #3
  store i1 true, ptr %cleanup.cond2, align 1
  %coerce.dive = getelementptr inbounds %struct.trivial, ptr %agg.tmp, i32 0, i32 0
  %1 = load ptr, ptr %coerce.dive, align 8
  invoke void @_Z6callee7trivial5other(ptr %1, ptr noundef %agg.tmp1)
          to label %invoke.cont unwind label %lpad

lpad:                                             ; preds = %cond.true
  %2 = landingpad { ptr, i32 }
          cleanup
  ; ... exception logistics
  %cleanup.is_active3 = load i1, ptr %cleanup.cond2, align 1       ; = true
  br i1 %cleanup.is_active3, label %cleanup.action4, label %cleanup.done5

cleanup.action4:                                  ; preds = %lpad
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1) #3
  br label %cleanup.done5

cleanup.done5:                                    ; preds = %cleanup.action4, %lpad
  %cleanup.is_active6 = load i1, ptr %cleanup.cond, align 1        ; = true
  br i1 %cleanup.is_active6, label %cleanup.action7, label %cleanup.done8

cleanup.action7:                                  ; preds = %cleanup.done5
  call void @_ZN7trivialD1Ev(ptr noundef nonnull align 8 dereferenceable(8) %agg.tmp) #3
  br label %cleanup.done8

cleanup.done8:                                    ; preds = %cleanup.action7, %cleanup.done5
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %n) #3
  call void @_ZN7trivialD1Ev(ptr noundef nonnull align 8 dereferenceable(8) %f) #3
  br label %eh.resume

eh.resume:                                        ; preds = %cleanup.done8
  ; ... exception logistics
  resume { ptr, i32 } %lpad.val9
}
```
So if `caller` throws, we destruct both parameters (and both locals). But we don't own the first parameter anymore after the call. Inspecting `callee` shows that it calls `_ZN7trivialD1Ev` on unwinding the exception that it has thrown. So we destruct the first parameter twice.

If we drop the ternary and unconditionally call `callee`, all is fine. We get:
```llvm
define dso_local void @_Z6callerv() #0 personality ptr @__gxx_personality_v0 {
entry:
  ; ... setting up locals
  call void @_ZN7trivialC1ERKS_(ptr noundef nonnull align 8 dereferenceable(8) %agg.tmp, ptr noundef nonnull align 8 dereferenceable(8) %f) #3
  call void @_ZN5otherC1ERKS_(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1, ptr noundef nonnull align 1 dereferenceable(1) %n) #3
  %coerce.dive = getelementptr inbounds %struct.trivial, ptr %agg.tmp, i32 0, i32 0
  %0 = load ptr, ptr %coerce.dive, align 8
  invoke void @_Z6callee7trivial5other(ptr %0, ptr noundef %agg.tmp1)
          to label %invoke.cont unwind label %lpad

lpad:                                             ; preds = %entry
  %1 = landingpad { ptr, i32 }
          cleanup
  ; ... exception logistics
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1) #3
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %n) #3
  call void @_ZN7trivialD1Ev(ptr noundef nonnull align 8 dereferenceable(8) %f) #3
  br label %eh.resume

eh.resume:                                        ; preds = %lpad
  ; ... exception logistics
  resume { ptr, i32 } %lpad.val2
}
```

It gets more interesting if we drop `noexcept` from `other`'s copy constructor:
```llvm
define dso_local void @_Z6callerv() #0 personality ptr @__gxx_personality_v0 {
entry:
  ; ... setting up locals
  store i1 false, ptr %cleanup.cond, align 1
  store i1 false, ptr %cleanup.cond2, align 1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  call void @_ZN7trivialC1ERKS_(ptr noundef nonnull align 8 dereferenceable(8) %agg.tmp, ptr noundef nonnull align 8 dereferenceable(8) %f) #4
  store i1 true, ptr %cleanup.cond, align 1
  invoke void @_ZN5otherC1ERKS_(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1, ptr noundef nonnull align 1 dereferenceable(1) %n)
          to label %invoke.cont unwind label %lpad

invoke.cont:                                      ; preds = %cond.true
  store i1 true, ptr %cleanup.cond2, align 1
  %coerce.dive = getelementptr inbounds %struct.trivial, ptr %agg.tmp, i32 0, i32 0
  %1 = load ptr, ptr %coerce.dive, align 8
  store i1 false, ptr %cleanup.cond, align 1                         ; !!!
  invoke void @_Z6callee7trivial5other(ptr %1, ptr noundef %agg.tmp1)
          to label %invoke.cont4 unwind label %lpad3

lpad:                                             ; preds = %cond.true
  %2 = landingpad { ptr, i32 }
          cleanup
  ; ... exception logistics
  br label %ehcleanup

lpad3:                                            ; preds = %invoke.cont
  %5 = landingpad { ptr, i32 }
          cleanup
  ; ... exception logistics
  %cleanup.is_active5 = load i1, ptr %cleanup.cond2, align 1      ; = true
  br i1 %cleanup.is_active5, label %cleanup.action6, label %cleanup.done7

cleanup.action6:                                  ; preds = %lpad3
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %agg.tmp1) #4
  br label %cleanup.done7

cleanup.done7:                                    ; preds = %cleanup.action6, %lpad3
  br label %ehcleanup

ehcleanup:                                        ; preds = %cleanup.done7, %lpad
  %cleanup.is_active8 = load i1, ptr %cleanup.cond, align 1       ; = phi i1 [ false, %cleanup.done7 ], [ true, %lpad ]
  br i1 %cleanup.is_active8, label %cleanup.action9, label %cleanup.done10

cleanup.action9:                                  ; preds = %ehcleanup
  call void @_ZN7trivialD1Ev(ptr noundef nonnull align 8 dereferenceable(8) %agg.tmp) #4
  br label %cleanup.done10

cleanup.done10:                                   ; preds = %cleanup.action9, %ehcleanup
  call void @_ZN5otherD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %n) #4
  call void @_ZN7trivialD1Ev(ptr noundef nonnull align 8 dereferenceable(8) %f) #4
  br label %eh.resume

eh.resume:                                        ; preds = %cleanup.done10
  ; ... exception logistics
  resume { ptr, i32 } %lpad.val13
}
```
So after having initialized the second parameter, and before handing ownership of the `trivial_abi` object into the callee, we write `false` into `cleanup.cond`. So we only clean up if initializing the second parameter failed and we couldn't pass on the object.

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-codegen

---

### Comment 2 - aaronpuchert

CC @ahatanaka

---

### Comment 3 - aaronpuchert

Fixed with d72146f47156dc645b1c0a4cb9c72f01e6d6dd0e.

---

