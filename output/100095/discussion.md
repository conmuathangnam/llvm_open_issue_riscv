# Crash from Assertion `!Packs.empty() && "Pack expansion without unexpanded packs?"'

**Author:** ilya-biryukov
**URL:** https://github.com/llvm/llvm-project/issues/100095

## Body

The following code crashes Clang with assertion at head, see https://gcc.godbolt.org/z/j6zY4as9h
```cpp
template <class... T>
struct Types {};
template <int& field>
using Forget = int;
template <int&... fields>
using SeqKey = Types<Forget<fields>...>;

template <typename Key, typename Value>
struct HelperBase {
  using ResponseParser = Key();
  HelperBase(ResponseParser response_parser) {}
};
template <int&... fields>
SeqKey<fields...> Parser();

template <int&... fields>
struct Helper : public HelperBase<SeqKey<fields...>, double> {
  using Key = SeqKey<fields...>;
  using Value = double;
  using ParentClass = HelperBase<Key, Value>;
  Helper() : ParentClass(Parser<fields...>) {}
};
void test() { Helper<>(); }
```

This should be relatively recent as it was found by our release testing.

## Comments

### Comment 1 - ilya-biryukov

@alexfh says it might be old, actually, we only found it because we started building the compiler with libc++ hardening.
He pointed out that the code was added in
 https://github.com/llvm/llvm-project/commit/4a8f3518cb1444f7041ecb36fceb49d59f08b4b1
and  it crashes on this line when assertions are disabled: https://github.com/llvm/llvm-project/blob/363e036ac002d5af4bb82e303052b806a98086a1/clang/lib/Sema/SemaTemplateDeduction.cpp#L957

---

### Comment 2 - alexfh

@zygoloid I wonder if you remember this code and could help resolving this UB and the assertion.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ilya Biryukov (ilya-biryukov)

<details>
The following code crashes Clang with assertion at head, see https://gcc.godbolt.org/z/j6zY4as9h
```cpp
template &lt;class... T&gt;
struct Types {};
template &lt;int&amp; field&gt;
using Forget = int;
template &lt;int&amp;... fields&gt;
using SeqKey = Types&lt;Forget&lt;fields&gt;...&gt;;

template &lt;typename Key, typename Value&gt;
struct HelperBase {
  using ResponseParser = Key();
  HelperBase(ResponseParser response_parser) {}
};
template &lt;int&amp;... fields&gt;
SeqKey&lt;fields...&gt; Parser();

template &lt;int&amp;... fields&gt;
struct Helper : public HelperBase&lt;SeqKey&lt;fields...&gt;, double&gt; {
  using Key = SeqKey&lt;fields...&gt;;
  using Value = double;
  using ParentClass = HelperBase&lt;Key, Value&gt;;
  Helper() : ParentClass(Parser&lt;fields...&gt;) {}
};
void test() { Helper&lt;&gt;(); }
```

This should be relatively recent as it was found by our release testing.
</details>


---

### Comment 4 - ilya-biryukov

I got to the bottom of it, we are loosing any mention of `fields` from the type alias `using SeqKey = Types<Forget<fields>...>` when deducing it against `Types<>`. We end up deducing `Types<int...>` against `Types<>` and the code fails, because it (rightfully) assumes that `PackExpansionType` should have some unexpanded types in its pattern. We would probably want to deduce against `Types<Forget<fields>...>` instead, but that's the tricky part of template instantiations that always confused me (on one hand `Forget<fields>` has to be substituted when written, in the template itself; on the other hand, `fields` is an unexpanded parameter pack and if we loose it, the pack expansions type does not make sense).

See draft #100144 for (an invalid) fix that bring some other problems with it, I'll need to explore something along those lines for a proper fix.
@zygoloid if you have directions, that'd be helpful.

---

### Comment 5 - cor3ntin

Ping @mizvekov, our sugar expert :)

---

### Comment 6 - mizvekov

I think in particular anything that relied on type deduction preserving sugar would be extremely fragile.

The fix in #100144 is invalid. We can't use as-written template arguments where resolved arguments are expected, this will crash in a bunch of places.

To fix that particular sugar loss in type deduction, we need sugared resolved arguments, which is implemented by https://reviews.llvm.org/D134113
Though that patch in particular had a non-trivial impact on compile-time-tracker tests.

![image](https://github.com/user-attachments/assets/fc16e30e-5e06-4b8a-9f28-86c65fa767dc)

This might be fine as is, but It just so always happens that google will have some internal thing where the impact is enormous. I have some ideas to reduce impact in that case.

I intend to pick up that patch again soon after I restart my resugaring work, which is after my current thing is done.

---

### Comment 7 - ilya-biryukov

> The fix in #100144 is invalid. We can't use as-written template arguments where resolved arguments are expected, this will crash in a bunch of places.

Yep, fully agree, the description of the PR says exactly that and that's why in draft state.

> To fix that particular sugar loss in type deduction, we need sugared resolved arguments, which is implemented by https://reviews.llvm.org/D134113 Though that patch in particular had a non-trivial impact on compile-time-tracker tests.

Could you elaborate on this? Do we have other similar cases for deduction that work with sugar in Clang?
I am a little surprised that we need to rely on sugar over resolved templates for correctness, I have previously assumed that the sugar is there for improved diagnostics, tooling and non-standard static analysis (e.g. to track nullability and other attributes), but it would not affect correctness. E.g. we would have things like `DecltypeType` for sugar on non-dependent expressions and keep it as a canonical type for dependent expression.

> This might be fine as is, but It just so always happens that google will have some internal thing where the impact is enormous. I have some ideas to reduce impact in that case.

I work at Google, and I am happy to check we're not seeing any compilation timeouts (i.e. enormous impact). Although our infrastructure does not allow to check how much slower the compilation will get until this actually gets released and we can stare at the graphs monitoring our build latencies, etc. That's still something, though, and given it's a correctness fix, we would be willing to take some regression and help reduce it.

---

### Comment 8 - mizvekov

> Could you elaborate on this? Do we have other similar cases for deduction that work with sugar in Clang? I am a little surprised that we need to rely on sugar over resolved templates for correctness, I have previously assumed that the sugar is there for improved diagnostics, tooling and non-standard static analysis (e.g. to track nullability and other attributes), but it would not affect correctness. E.g. we would have things like `DecltypeType` for sugar on non-dependent expressions and keep it as a canonical type for dependent expression.
> 

Until about a three years ago, type deduction in clang did not preserve any type sugar at all.
We started implementing it in the context of `auto` type deduction: https://reviews.llvm.org/D110216
And slowly improved it over time.

But we missed preserving type sugar in type deduction through template specializations, mainly because
it's a more massive change, with bigger compile time impact.

After that is taken care of, I think we would be preserving it for practically all cases in type deduction itself.
But there might be a few corner cases we missed, and this could have a long tail until we find everything.

But in general, we have other issues, for one aspect, how we represent some semantic adjustments.
We are quite trigger happy to eliminate type sugar when we need unqualified types, for one.
For another, some adjustments where we need to lose reference types also lose sugar.
We have a mechanism to express these adjustments without losing sugar, but it's a bit expensive and we don't use it everywhere.

So code in clang that relies on type sugar in order not to crash is highly suspect.
I mean if it would be obvious we handle all cases in that particular situation, that would be fine, but it's hardly ever obvious. If it has to go through type deduction, that would be a massive NO.

> I work at Google, and I am happy to check we're not seeing any compilation timeouts (i.e. enormous impact). Although our infrastructure does not allow to check how much slower the compilation will get until this actually gets released and we can stare at the graphs monitoring our build latencies, etc. That's still something, though, and given it's a correctness fix, we would be willing to take some regression and help reduce it.

Thanks.
Though it shouldn't be about correctness in the sense of producing valid programs, but about producing better quality diagnostics and improving some kinds of analysis.


---

### Comment 9 - mizvekov

By the way, the change which introduced the crash, https://github.com/llvm/llvm-project/commit/4a8f3518cb1444f7041ecb36fceb49d59f08b4b1, is much older than when we started preserving sugar in type deduction. So it's unlikely the change intended to rely on it.

---

### Comment 10 - alexfh

Any ideas on whether we could quick fix the UB in clang and just preserve the current behavior in non-assertions-enabled Clang build?

---

### Comment 11 - ilya-biryukov

> By the way, the change which introduced the crash, https://github.com/llvm/llvm-project/commit/4a8f3518cb1444f7041ecb36fceb49d59f08b4b1, is much older than when we started preserving sugar in type deduction. So it's unlikely the change intended to rely on it.

Yes, I think it's a correctness issue that was there disregarding the sugar. If my reading of your previous comment is correct, I think the type sugar in itself is not the right way to solve this, I'll dig a bit deeper to understand what the standard says we should do here (technically we should substitute the type alias inside the template, but it's unclear whether deduction should still "see" the type alias in any way).

> So code in clang that relies on type sugar in order not to crash is highly suspect.

That's the part that I wanted to focus on in particular. It's also true that anything that relying on type sugar for correctness of deduction is also highly suspect, right?

> Any ideas on whether we could quick fix the UB in clang and just preserve the current behavior in non-assertions-enabled Clang build?

I think I have an idea, but that revolves around removing the assumption that pack expansion type necessarily contains an unexpanded pack. From the looks of it, there is definitely quite some code that relies on it, so it may turn out to be non-trivial.

---

### Comment 12 - alexfh

> I think I have an idea, but that revolves around removing the assumption that pack expansion type necessarily contains an unexpanded pack. From the looks of it, there is definitely quite some code that relies on it, so it may turn out to be non-trivial.

I mean a really quick fix that just retains current behavior with the libc++ runtime checks enabled, e.g. https://github.com/llvm/llvm-project/pull/100408

---

### Comment 13 - ilya-biryukov

MSVC and GCC just tend to deduce empty template arguments from pack expansions that don't mention the template parameter name, resulting in "too many arguments" warning:
https://gcc.godbolt.org/z/T7obrebKs

Clang does something unreasonable and deduces a template pack with `<no value>` placeholder types.
The expected behavior when the template arguments are specified explicitly is clear: just use those arguments.
The expected behavior when there are no template arguments specified that MSVC and GCC have is interesting: they will simply deduce the pack to have 0 length. 

Btw, a simpler example that does not involve a type alias:
https://gcc.godbolt.org/z/PzsGEe8Yv
```cpp
template <class ...T> struct Bar {};
template <class ...T> void bar(typename Bar<T>::type...);

void test() {
    bar(); 
    bar(1,1,1);  // GCC and MSVC deduce empty argument list, Clang deduces "<no value>, <no value>, <no value>".
}
```

I think I'll check back with the standard to see what's expected.

---

### Comment 14 - ilya-biryukov

> I mean a really quick fix that just retains current behavior with the libc++ runtime checks enabled, e.g. #100408

I would rather get a proper fix, but having a defined-but-broken is probably better than UB.
I'm not sure it's all UB you can get from it, though, there are other asserts failing (like the one in this bug).

We're still treating the symptoms and not the root cause.

---

### Comment 15 - alexfh

> We're still treating the symptoms and not the root cause.

That's exactly what I need short term - to be able to turn on libc++ runtime checks (specifically, `-D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_FAST`) for clang and avoid possible backsliding. Unfortunately, this bug makes clang unusable with libc++ hardening. Thus, I'm looking for a quick (possibly, temporary) solution until a good one is in place.

---

### Comment 16 - mizvekov

> Any ideas on whether we could quick fix the UB in clang and just preserve the current behavior in non-assertions-enabled Clang build?

One idea is to ask libc++ folks if they would be able to work around this problem on their side. @ldionne 

> Yes, I think it's a correctness issue that was there disregarding the sugar. If my reading of your previous comment is correct, I think the type sugar in itself is not the right way to solve this, I'll dig a bit deeper to understand what the standard says we should do here (technically we should substitute the type alias inside the template, but it's unclear whether deduction should still "see" the type alias in any way).

Yes, I don't think we are nowhere soon, if ever, going to get to the point where we can enforce sugar preservation everywhere in C++ from an architectural standpoint. So anything that relied on sugar, either not to crash or to produce semantic effect, would be inconsistent with clang architecture.

Of course there were some inconsistent decisions here, like allowing `_Nonnull` and `[[gcc::aligned(X)]]` in C++ with no restrictions, which attach semantic effect to sugar.

Outside of C++, there are some sugar stuffs that are guaranteed to work and relied on semantically. Particularly in ObjC.
But C++ adds too many problems here.

> 
> That's the part that I wanted to focus on in particular. It's also true that anything that relying on type sugar for correctness of deduction is also highly suspect, right?

For deduction yes, since we started with no sugar preservation in deduction, and then improved this, but never got to the point where we could rely on it.

Btw, I wouldn't rule out that the current pack implementation in clang may incorrectly rely on sugar in some places.
I wouldn't rule out either that the standard might have defects which makes this hard to avoid.

---

### Comment 17 - alexfh

> One idea is to ask libc++ folks if they would be able to work around this problem on their side. @ldionne

I don't see anything wrong with libc++ - it just implements a hardening mode where an access to an unset optional aborts the program (or whatever `_LIBCPP_ASSERTION_HANDLER` does in a specific deployment). The problem is that certain code (a reduced example of which @ilya-biryukov posted) triggers this assertion in Clang.

---

### Comment 18 - mizvekov

> I don't see anything wrong with libc++ - it just implements a hardening mode where an access to an unset optional aborts the program (or whatever `_LIBCPP_ASSERTION_HANDLER` does in a specific deployment). The problem is that certain code (a reduced example of which @ilya-biryukov posted) triggers this assertion in Clang.

I fully believe that. But I do also believe libc++ implements workarounds for cases clang is faulty.
libc++ folks could even be interested in this workaround themselves, since they need to support different compiler versions.
So their opinion would be highly welcome :)

---

### Comment 19 - alexfh

@mizvekov I'm not sure I understand what kind of a libc++ workaround would be possible here. We're talking about `std::optional<>::operator->` aborting program (in this case, Clang), when the optional has no value.  The code that triggers the error in Clang, is in no way related to libc++. 

---

### Comment 20 - mizvekov

The code somehow exercises a code path we had never before seen a test case for.

I spent some time in the past trying to reverse engineer a test case that would hit it.
Though I gave up after deciding it would take too long to be worth it.

My gut feeling is that this is no ordinary code.
But if the libc++ folks are like 'I have no idea', in reasonable time, I'd not oppose your plan.

---

### Comment 21 - ilya-biryukov

To reiterate what Alex said, there is no actual code in libc++ that hits this, but the assertions inside libc++ from hardened mode uncovers a bug in Clang. It happens on real code (similar to the one I filed in this issue), but the code is outside of libc++.

I think I have a good hold of what could be done here and plan to dig deeper into it and send a proper fix.
We could either land or not land #100408 in the meanwhile, I don't have strong opinions about that. We are hitting a UB with our code, but I don't think it's much different from hitting various assertion failures that we occasionally get hit by in our large monorepo.

---

### Comment 22 - alexfh

Another test case that causes the same assertion failure:
```
template <class...>
class tuple;
template <class _Fp>
void function(_Fp);
template <char>
using ElementType = char;
template <char... fields>
tuple<ElementType<fields>...> P();
void g() {
  function(P<>);
}
```
https://gcc.godbolt.org/z/zvToKrb4v

Also looks similar to https://github.com/llvm/llvm-project/issues/112096.

---

### Comment 23 - mizvekov

So with #106730 both these reproducers stop crashing.

I saw that PR changes the test expectation for the test cases added for this, but I can't see any information about this at all.

Was this unexpected?

---

### Comment 24 - ilya-biryukov

@mizvekov there was a discussion about this in the review somewhere, but finding a link now would be challenging...
This was expected and it's on my list of TODOs to follow up and check that other examples from this issue don't crash either, add them to the test suite and close this issue. (Or land additional fixes if there's anything still missing)

---

