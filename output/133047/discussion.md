# [Clang] template with NTTP param is static array element failed to compile

**Author:** wanghenshui
**URL:** https://github.com/llvm/llvm-project/issues/133047
**Status:** Closed
**Labels:** clang:frontend, rejects-valid
**Closed Date:** 2025-03-27T06:15:23Z

## Body

it's code from CV-cuda, migrate to clang

```c++
#include <cstddef>
#include <stdint.h>
#include <utility>
#include <tuple>
template<bool Active = true, typename T>
constexpr inline bool IsOutside(T c, T s)
{
    if constexpr (Active)
    {
        return (c < 0) || (c >= s);
    }
    return false;
}


template<bool... ActiveDimensions>
class BoolDimension
{
public:
    static constexpr int  kNumDimensions = sizeof...(ActiveDimensions);
    struct ActiveMap
    {
        bool from[kNumDimensions];

        constexpr ActiveMap()
            : from()
        {
            int j = 0;
            for (int i = 0; i < kNumDimensions; ++i)
            {
                if (kActiveDimensions[i])
                {
                    from[i] = j++;
                }
            }
        }
    };

    static constexpr ActiveMap kMap{};
    static constexpr bool kActiveDimensions[]  = {ActiveDimensions...};
    static constexpr int  kNumActiveDimensions = ((ActiveDimensions ? 1 : 0) + ...);
    int64_t          m_tensorShape[kNumActiveDimensions] = {0};

    template<typename... Args, std::size_t... Is>
    inline  bool check(std::index_sequence<Is...>, Args... c) const
    {
        //static constexpr const  auto kDimensions = std::make_tuple(ActiveDimensions...);
        //if ((IsOutside<std::get<Is>(kDimensions)>(c,false) || ...))
        if ((IsOutside<kActiveDimensions[Is]>(c,false) || ...))
        {
            return true;
        }
        return false;
    }
};

int main() {
    BoolDimension<true, true, false, false> d;
    constexpr auto Is = std::make_index_sequence<1>{};
    return d.check(Is, false);
}
```

in check function, IsOutside not accept array element, change to tuple with get is ok

https://godbolt.org/z/n1K5vGcrE



## Comments

### Comment 1 - wanghenshui

https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2647r1.html

use lambda workaround also ok

---

### Comment 2 - frederick-vs-ja

Clang correctly accepts this with the following change ([Godbolt link](https://godbolt.org/z/8xa7e4c1o)):
```diff
-    static constexpr bool kActiveDimensions[]  = {ActiveDimensions...};
+    static constexpr bool kActiveDimensions[kNumDimensions]  = {ActiveDimensions...};
```

This is probably duplicate of either #79750 or #113936.


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ted Mostly (wanghenshui)

<details>
it's code from CV-cuda, migrate to clang

```c++
#include &lt;cstddef&gt;
#include &lt;stdint.h&gt;
#include &lt;utility&gt;
#include &lt;tuple&gt;
template&lt;bool Active = true, typename T&gt;
constexpr inline bool IsOutside(T c, T s)
{
    if constexpr (Active)
    {
        return (c &lt; 0) || (c &gt;= s);
    }
    return false;
}


template&lt;bool... ActiveDimensions&gt;
class BoolDimension
{
public:
    static constexpr int  kNumDimensions = sizeof...(ActiveDimensions);
    struct ActiveMap
    {
        bool from[kNumDimensions];

        constexpr ActiveMap()
            : from()
        {
            int j = 0;
            for (int i = 0; i &lt; kNumDimensions; ++i)
            {
                if (kActiveDimensions[i])
                {
                    from[i] = j++;
                }
            }
        }
    };

    static constexpr ActiveMap kMap{};
    static constexpr bool kActiveDimensions[]  = {ActiveDimensions...};
    static constexpr int  kNumActiveDimensions = ((ActiveDimensions ? 1 : 0) + ...);
    int64_t          m_tensorShape[kNumActiveDimensions] = {0};

    template&lt;typename... Args, std::size_t... Is&gt;
    inline  bool check(std::index_sequence&lt;Is...&gt;, Args... c) const
    {
        //static constexpr const  auto kDimensions = std::make_tuple(ActiveDimensions...);
        //if ((IsOutside&lt;std::get&lt;Is&gt;(kDimensions)&gt;(c,false) || ...))
        if ((IsOutside&lt;kActiveDimensions[Is]&gt;(c,false) || ...))
        {
            return true;
        }
        return false;
    }
};

int main() {
    BoolDimension&lt;true, true, false, false&gt; d;
    constexpr auto Is = std::make_index_sequence&lt;1&gt;{};
    return d.check(Is, false);
}
```

in check function, IsOutside not accept array element, change to tuple with get is ok

https://godbolt.org/z/n1K5vGcrE


</details>


---

