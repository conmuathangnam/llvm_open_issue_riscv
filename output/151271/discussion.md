# Regression caused by  #143096

**Author:** tstellar
**URL:** https://github.com/llvm/llvm-project/issues/151271
**Status:** Closed
**Labels:** clang:frontend, regression, concepts
**Closed Date:** 2025-07-30T06:07:07Z

## Body

I've found a regression caused by #143096, it triggers a static_assert in the webkitgtk code base:

```
repro.ii:19:21: error: static assertion failed due to requirement '!std::is_same_v<WebCore::SVGCircleElement, WebCore::SVGCircleElement>': Element should use fast property path
   19 |       static_assert(!std::is_same_v<OwnerType, SVGCircleElement>,
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
repro.ii:22:40: note: in instantiation of member function 'WebCore::SVGPropertyOwnerRegistry<WebCore::SVGCircleElement>::fastAnimatedPropertyLookup' requested here
   22 |   int synchronize(const int &) const { fastAnimatedPropertyLookup; }
      |                                        ^
repro.ii:25:46: note: in instantiation of member function 'WebCore::SVGPropertyOwnerRegistry<WebCore::SVGCircleElement>::synchronize' requested here
   25 |   SVGPropertyOwnerRegistry<SVGCircleElement> isKnownAttribute;
      |                      
```

Here is a reduced test case:

```
namespace std {
template <typename _Tp, typename _Up>
constexpr bool is_same_v = __is_same(_Tp, _Up);
}
namespace WebCore {
struct SVGPropertyRegistry {
  virtual int synchronize(const int &) const;
};
class SVGCircleElement;
template <typename T>
concept HasFastPropertyForAttribute =
    requires(T element, int name) { element.propertyForAttribute(name); };
template <typename OwnerType>
struct SVGPropertyOwnerRegistry : SVGPropertyRegistry {
  static void fastAnimatedPropertyLookup() {
    if constexpr (HasFastPropertyForAttribute<OwnerType>)
      ;
    else
      static_assert(!std::is_same_v<OwnerType, SVGCircleElement>,
                    "Element should use fast property path");
  }
  int synchronize(const int &) const { fastAnimatedPropertyLookup; }
};
void svgAttributeChanged() {
  SVGPropertyOwnerRegistry<SVGCircleElement> isKnownAttribute;
}
class SVGCircleElement {
  friend SVGPropertyOwnerRegistry<SVGCircleElement>;
  void propertyForAttribute(int);
};
} // namespace WebCore
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tom Stellard (tstellar)

<details>
I've found a regression caused by #<!-- -->143096, it triggers a static_assert in the webkitgtk code base:

```
repro.ii:19:21: error: static assertion failed due to requirement '!std::is_same_v&lt;WebCore::SVGCircleElement, WebCore::SVGCircleElement&gt;': Element should use fast property path
   19 |       static_assert(!std::is_same_v&lt;OwnerType, SVGCircleElement&gt;,
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
repro.ii:22:40: note: in instantiation of member function 'WebCore::SVGPropertyOwnerRegistry&lt;WebCore::SVGCircleElement&gt;::fastAnimatedPropertyLookup' requested here
   22 |   int synchronize(const int &amp;) const { fastAnimatedPropertyLookup; }
      |                                        ^
repro.ii:25:46: note: in instantiation of member function 'WebCore::SVGPropertyOwnerRegistry&lt;WebCore::SVGCircleElement&gt;::synchronize' requested here
   25 |   SVGPropertyOwnerRegistry&lt;SVGCircleElement&gt; isKnownAttribute;
      |                      
```

Here is a reduced test case:

```
namespace std {
template &lt;typename _Tp, typename _Up&gt;
constexpr bool is_same_v = __is_same(_Tp, _Up);
}
namespace WebCore {
struct SVGPropertyRegistry {
  virtual int synchronize(const int &amp;) const;
};
class SVGCircleElement;
template &lt;typename T&gt;
concept HasFastPropertyForAttribute =
    requires(T element, int name) { element.propertyForAttribute(name); };
template &lt;typename OwnerType&gt;
struct SVGPropertyOwnerRegistry : SVGPropertyRegistry {
  static void fastAnimatedPropertyLookup() {
    if constexpr (HasFastPropertyForAttribute&lt;OwnerType&gt;)
      ;
    else
      static_assert(!std::is_same_v&lt;OwnerType, SVGCircleElement&gt;,
                    "Element should use fast property path");
  }
  int synchronize(const int &amp;) const { fastAnimatedPropertyLookup; }
};
void svgAttributeChanged() {
  SVGPropertyOwnerRegistry&lt;SVGCircleElement&gt; isKnownAttribute;
}
class SVGCircleElement {
  friend SVGPropertyOwnerRegistry&lt;SVGCircleElement&gt;;
  void propertyForAttribute(int);
};
} // namespace WebCore
```
</details>


---

### Comment 2 - zyn0217

Reduced further: https://gcc.godbolt.org/z/jnhzWj3xn

---

