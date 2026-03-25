# [Clang] clang 22 still has problems with mp_units

**Author:** HazardyKnusperkeks
**URL:** https://github.com/llvm/llvm-project/issues/175831

## Body

Similar to #170856 Clang 22 fails with a simple example: https://godbolt.org/z/c7vE8x6Wh which is accepted by GCC and Clang 21. It was also broken by e9972debc98ce5d00db47409248bbcf06fafaf73 (#161671), but not fixed with a5bfe8e5c3c4c5bb46f53e093caf6aa01d56aeab (#172251).

The example (also used in the godbolt link):
``` c++
#include <mp-units/systems/isq/space_and_time.h>
#include <mp-units/systems/si/unit_symbols.h>
#include <mp-units/systems/si/units.h>

using namespace mp_units::si::unit_symbols;

mp_units::quantity distance = mp_units::isq::distance(5 * km);
```

The reported error:
```
<source>:7:31: error: no matching function for call to object of type 'const struct distance'
    7 | mp_units::quantity distance = mp_units::isq::distance(5 * km);
      |                               ^~~~~~~~~~~~~~~~~~~~~~~
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:217:41: note: candidate template ignored: constraints not satisfied [with Self = struct distance, FwdQ = quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>, Q = std::remove_cvref_t<quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>>]
  217 |   [[nodiscard]] constexpr Quantity auto operator()(this Self, FwdQ&& q)
      |                                         ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:216:33: note: because 'typename quantity<make_reference(Self{}, Q::unit), typename Q::rep>' would be invalid: constraints not satisfied for class template 'quantity' [with R = reference<mp_units::isq::distance, mp_units::si::kilo_<mp_units::si::metre>>{}, Rep = typename std::remove_cvref_t<quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>>::rep]
  216 |             requires { typename quantity<make_reference(Self{}, Q::unit), typename Q::rep>; }
      |                                 ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:226:46: note: candidate template ignored: constraints not satisfied [with Self = struct distance, FwdQP = quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>, QP = std::remove_cvref_t<quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>>]
  226 |   [[nodiscard]] constexpr QuantityPoint auto operator()(this Self self, FwdQP&& qp)
      |                                              ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:223:43: note: because 'std::remove_cvref_t<quantity<mp_units::si::kilo_<mp_units::si::metre>{}, int>>' (aka 'mp_units::quantity<mp_units::si::kilo_<mp_units::si::metre>{{{{}, {}}}}, int>') does not satisfy 'QuantityPoint'
  223 |   template<typename Self, typename FwdQP, QuantityPoint QP = std::remove_cvref_t<FwdQP>>
      |                                           ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_point_concepts.h:51:25: note: because 'detail::is_quantity_point<mp_units::quantity<mp_units::si::kilo_<mp_units::si::metre>{{{{}, {}}}}, int>>' evaluated to false
   51 | concept QuantityPoint = detail::is_quantity_point<T>;
      |                         ^
1 error generated.
Compiler returned: 1
```

I know it's not minimal, but I honestly don't know how to minimize it, maybe @mpusz can help?

CC @cor3ntin @zyn0217 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Björn Schäpers (HazardyKnusperkeks)

<details>
Similar to #<!-- -->170856 Clang 22 fails with a simple example: https://godbolt.org/z/c7vE8x6Wh which is accepted by GCC and Clang 21. It was also broken by e9972debc98ce5d00db47409248bbcf06fafaf73 (#<!-- -->161671), but not fixed with a5bfe8e5c3c4c5bb46f53e093caf6aa01d56aeab (#<!-- -->172251).

The example (also used in the godbolt link):
``` c++
#include &lt;mp-units/systems/isq/space_and_time.h&gt;
#include &lt;mp-units/systems/si/unit_symbols.h&gt;
#include &lt;mp-units/systems/si/units.h&gt;

using namespace mp_units::si::unit_symbols;

mp_units::quantity distance = mp_units::isq::distance(5 * km);
```

The reported error:
```
&lt;source&gt;:7:31: error: no matching function for call to object of type 'const struct distance'
    7 | mp_units::quantity distance = mp_units::isq::distance(5 * km);
      |                               ^~~~~~~~~~~~~~~~~~~~~~~
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:217:41: note: candidate template ignored: constraints not satisfied [with Self = struct distance, FwdQ = quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;, Q = std::remove_cvref_t&lt;quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;&gt;]
  217 |   [[nodiscard]] constexpr Quantity auto operator()(this Self, FwdQ&amp;&amp; q)
      |                                         ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:216:33: note: because 'typename quantity&lt;make_reference(Self{}, Q::unit), typename Q::rep&gt;' would be invalid: constraints not satisfied for class template 'quantity' [with R = reference&lt;mp_units::isq::distance, mp_units::si::kilo_&lt;mp_units::si::metre&gt;&gt;{}, Rep = typename std::remove_cvref_t&lt;quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;&gt;::rep]
  216 |             requires { typename quantity&lt;make_reference(Self{}, Q::unit), typename Q::rep&gt;; }
      |                                 ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:226:46: note: candidate template ignored: constraints not satisfied [with Self = struct distance, FwdQP = quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;, QP = std::remove_cvref_t&lt;quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;&gt;]
  226 |   [[nodiscard]] constexpr QuantityPoint auto operator()(this Self self, FwdQP&amp;&amp; qp)
      |                                              ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_spec.h:223:43: note: because 'std::remove_cvref_t&lt;quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{}, int&gt;&gt;' (aka 'mp_units::quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{{{{}, {}}}}, int&gt;') does not satisfy 'QuantityPoint'
  223 |   template&lt;typename Self, typename FwdQP, QuantityPoint QP = std::remove_cvref_t&lt;FwdQP&gt;&gt;
      |                                           ^
/opt/compiler-explorer/libs/mp-units/trunk/src/core/include/mp-units/framework/quantity_point_concepts.h:51:25: note: because 'detail::is_quantity_point&lt;mp_units::quantity&lt;mp_units::si::kilo_&lt;mp_units::si::metre&gt;{{{{}, {}}}}, int&gt;&gt;' evaluated to false
   51 | concept QuantityPoint = detail::is_quantity_point&lt;T&gt;;
      |                         ^
1 error generated.
Compiler returned: 1
```

I know it's not minimal, but I honestly don't know how to minimize it, maybe @<!-- -->mpusz can help?

CC @<!-- -->cor3ntin @<!-- -->zyn0217 
</details>


---

### Comment 2 - cor3ntin

"reduced" to https://godbolt.org/z/9cr5EjoGa 

---

### Comment 3 - frederick-vs-ja

A more reduced example ([Godbolt link](https://godbolt.org/z/jnqv1rbv4)):
```C++
template<class>
struct reference {};
template<class Q>
consteval Q get_spec(reference<Q>) { return {}; }

template<class T>
concept repr_impl = sizeof(T) > 0;
template<class, auto V>
concept representation_of = repr_impl<decltype(V)>;
template<auto V, representation_of<::get_spec(V)>>
struct quantity {};

auto x = quantity<reference<int>{}, int>{};
```

---

### Comment 4 - zyn0217

When checking constraints, we now evaluate `get_spec` within an unevaluated context `decltype(...)`, and therefore we will not instantiate any special members of `reference<int>`, which are necessary for the evaluation of constant expressions, which the evaluation is requested when we transform a SubstNTTP expression.

@cor3ntin @mizvekov I tried to get rid of sugars for constraints, but it doesn't work. Maybe we should add some workaround in CheckTemplateArgument but I don't know

---

### Comment 5 - mpusz

Maybe you can add mp-units to your compiler's unit tests suits? It is quite challenging library to handle and we had clang regressions many times already (i.e., clang-19 even decided to not fix the regression so mp-units is not supported there).

---

### Comment 6 - shafik

> Maybe you can add mp-units to your compiler's unit tests suits? It is quite challenging library to handle and we had clang regressions many times already (i.e., clang-19 even decided to not fix the regression so mp-units is not supported there).

I am not sure what resources would be required to do that and how much work/time it would be CC @AaronBallman 

We definitely have holes in some areas of our test coverage and we definitely need people with deeper knowledge of those areas to help fill out our coverage. In the long run that is the real fix for these kinds of regressions. 

---

### Comment 7 - AaronBallman

> > Maybe you can add mp-units to your compiler's unit tests suits? It is quite challenging library to handle and we had clang regressions many times already (i.e., clang-19 even decided to not fix the regression so mp-units is not supported there).
> 
> I am not sure what resources would be required to do that and how much work/time it would be CC [@AaronBallman](https://github.com/AaronBallman)

I suspect [llvm-test-suite](https://github.com/llvm/llvm-test-suite) would be a good home for it to live; we have post-commit bots which run those tests but it's a place for external test suites to live.


---

### Comment 8 - HazardyKnusperkeks

Any chance there will be a fix before clang 22 is released?

---

### Comment 9 - AaronBallman

> Any chance there will be a fix before clang 22 is released?

CC @zyn0217 

Also, given that this is a regression, we should consider whether there's anything here which should be reverted to get back to the old behavior despite the disruptions. CC @cor3ntin @erichkeane for additional opinions

---

### Comment 10 - erichkeane

OOof... this is really rock -> hard place for me.  The initial patch to normalize constraints is fixing tons of incorrect checking of constraints.  I'd very much be against reverting it, the fallout from all the things depending on it is more than we could be expected to do at this point.

That said, this is a really painful regression.  @cor3ntin or @zyn0217 : Do we at least have a root-cause of some sort on this yet?  I think I'd be OK trying to figure out if this is something we can get into a point release, since I'm sure we've already missed the original release.

---

### Comment 11 - mpusz

It would be great to get a fix. Every time I add [a compiler version that does not work with mp-units](https://mpusz.github.io/mp-units/latest/getting_started/cpp_compiler_support/), my users are really upset and complain. We already had this huge issue with clang-19 and apple-clang-17 😢

---

### Comment 12 - zyn0217

> Do we at least have a root-cause of some sort on this yet?

https://github.com/llvm/llvm-project/issues/175831#issuecomment-3753235470

Based on the reduced example, it is 'expected' that checking `representation_of<::get_spec(V)>` doesn't instantiate any special members of `reference<int>` because we are in the non-evaluated context thanks to `decltype(V)`, which gets into the way before we substitute `<int>` into any of the concepts.

A transform of SubstNTTP requires a CheckTemplateArgument that ends up evaluating the constant expressions that needs the copy constructor. This is where we are tripped over.

I think a solution would be to either let TransformSubstNTTP not relying on `CheckTemplateArgument` (which is likely impossible because @mizvekov @cor3ntin and I discussed a similar regression and concluded we have to preserve `CheckTemplateArgument` calls, or find a way to bypass the constant evaluation. Neither seems feasible to me.

---

