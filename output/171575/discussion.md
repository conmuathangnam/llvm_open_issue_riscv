# Issue with lifetime extension of temporaries in default member initializers

**Author:** bartdesmet
**URL:** https://github.com/llvm/llvm-project/issues/171575

## Body

Given the following definitions:

```cpp
#include <optional>

struct s
{
    const std::optional<int>& x = std::nullopt;
};

struct t
{
    const s& v;
};
```

and the following usage:

```cpp
#include <iostream>

void f(const s& args)
{
    if (args.x)
    {
        std::cout << *args.x << std::endl;
    }
    else
    {
        std::cout << "N/A" << std::endl;
    }
}

void g(const t& args)
{
    f(args.v);
}
```

we're seeing a stack-use-after-return ASAN error for the following case:

```cpp
int main()
{
    const t args{.v = {}};
    g(args);
}
```

Looking at the LLVM IR, I'm seeing:

```
define dso_local void @test4()() local_unnamed_addr {
entry:
  %ref.tmp = alloca %struct.s, align 8
  %ref.tmp1 = alloca %"class.std::optional", align 4
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp) #6
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp1) #6
  store ptr %ref.tmp1, ptr %ref.tmp, align 8
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp1) #6
  call void @f(s const&)(ptr noundef nonnull align 8 dereferenceable(8) %ref.tmp)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp) #6
  ret void
}
```

where the lifetime of `ref.tmp1` does not extend.

With clang 19, this produced warning `lifetime extension of temporary created by aggregate initialization using a default member initializer is not yet supported; lifetime of temporary will end at the end of the full-expression`.

In contrast, the following works fine:

```cpp
int main()
{
    const s args{};
    f(args);
}
```

and lifetime of the default-initialized optional gets extended:

```
define dso_local void @test2()() local_unnamed_addr {
entry:
  %args = alloca %struct.s, align 8
  %ref.tmp = alloca %"class.std::optional", align 4
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %args) #6
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp) #6
  %_M_engaged.i.i.i.i = getelementptr inbounds i8, ptr %ref.tmp, i64 4
  store i8 0, ptr %_M_engaged.i.i.i.i, align 4
  store ptr %ref.tmp, ptr %args, align 8
  call void @f(s const&)(ptr noundef nonnull align 8 dereferenceable(8) %args)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp) #6
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %args) #6
  ret void
}
```

With GCC, lifetime seems to get extended.

I've shared a repro in godbolt at https://godbolt.org/z/PPPG6j53W. This has a custom `my_opt<T>` with a destructor that prints, to further illustrate the difference between GCC and clang. I.e.:

```cpp
    const t args{.v = {}};
    g(args);
```

prints

```
N/A
~my_opt
```

on GCC, i.e. destruction of the `std::nullopt` instance happens after the call to `g`, but prints

```
~my_opt
N/A
```

on clang.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Bart J.F. De Smet (bartdesmet)

<details>
Given the following definitions:

```cpp
#include &lt;optional&gt;

struct s
{
    const std::optional&lt;int&gt;&amp; x = std::nullopt;
};

struct t
{
    const s&amp; v;
};
```

and the following usage:

```cpp
#include &lt;iostream&gt;

void f(const s&amp; args)
{
    if (args.x)
    {
        std::cout &lt;&lt; *args.x &lt;&lt; std::endl;
    }
    else
    {
        std::cout &lt;&lt; "N/A" &lt;&lt; std::endl;
    }
}

void g(const t&amp; args)
{
    f(args.v);
}
```

we're seeing a stack-use-after-return ASAN error for the following case:

```cpp
int main()
{
    const t args{.v = {}};
    g(args);
}
```

Looking at the LLVM IR, I'm seeing:

```
define dso_local void @<!-- -->test4()() local_unnamed_addr {
entry:
  %ref.tmp = alloca %struct.s, align 8
  %ref.tmp1 = alloca %"class.std::optional", align 4
  call void @<!-- -->llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp) #<!-- -->6
  call void @<!-- -->llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp1) #<!-- -->6
  store ptr %ref.tmp1, ptr %ref.tmp, align 8
  call void @<!-- -->llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp1) #<!-- -->6
  call void @<!-- -->f(s const&amp;)(ptr noundef nonnull align 8 dereferenceable(8) %ref.tmp)
  call void @<!-- -->llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp) #<!-- -->6
  ret void
}
```

where the lifetime of `ref.tmp1` does not extend.

With clang 19, this produced warning `lifetime extension of temporary created by aggregate initialization using a default member initializer is not yet supported; lifetime of temporary will end at the end of the full-expression`.

In contrast, the following works fine:

```cpp
int main()
{
    const s args{};
    f(args);
}
```

and lifetime of the default-initialized optional gets extended:

```
define dso_local void @<!-- -->test2()() local_unnamed_addr {
entry:
  %args = alloca %struct.s, align 8
  %ref.tmp = alloca %"class.std::optional", align 4
  call void @<!-- -->llvm.lifetime.start.p0(i64 8, ptr nonnull %args) #<!-- -->6
  call void @<!-- -->llvm.lifetime.start.p0(i64 8, ptr nonnull %ref.tmp) #<!-- -->6
  %_M_engaged.i.i.i.i = getelementptr inbounds i8, ptr %ref.tmp, i64 4
  store i8 0, ptr %_M_engaged.i.i.i.i, align 4
  store ptr %ref.tmp, ptr %args, align 8
  call void @<!-- -->f(s const&amp;)(ptr noundef nonnull align 8 dereferenceable(8) %args)
  call void @<!-- -->llvm.lifetime.end.p0(i64 8, ptr nonnull %ref.tmp) #<!-- -->6
  call void @<!-- -->llvm.lifetime.end.p0(i64 8, ptr nonnull %args) #<!-- -->6
  ret void
}
```

With GCC, lifetime seems to get extended.

I've shared a repro in godbolt at https://godbolt.org/z/PPPG6j53W. This has a custom `my_opt&lt;T&gt;` with a destructor that prints, to further illustrate the difference between GCC and clang. I.e.:

```cpp
    const t args{.v = {}};
    g(args);
```

prints

```
N/A
~my_opt
```

on GCC, i.e. destruction of the `std::nullopt` instance happens after the call to `g`, but prints

```
~my_opt
N/A
```

on clang.
</details>


---

