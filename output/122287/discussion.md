# Incorrect "invalid conversion" error with decltype(<lambda>) in default template arguments

**Author:** ilya-biryukov
**URL:** https://github.com/llvm/llvm-project/issues/122287

## Body

The following code fails to compile (see https://godbolt.org/z/zTf49f9oe):
```cpp
template <typename MakeValue = decltype([] { return 1; })>
class Example {
   public:
    using Result = decltype(MakeValue()());
    explicit Example(MakeValue&& make_value = {})
        : make_value_(make_value) {}

    Result Make() { return make_value_(); }

   private:
     MakeValue make_value_;
};

template <typename CastTo = int>
CastTo CastResultTo() {
    auto example = Example();
    return example.Make(); // error: 'Cannot initialize object parameter'
}

int main() { return CastResultTo(); }
```

Adding an empty template argument list makes the error go away (see https://godbolt.org/z/dz7eMaM54):
```cpp
    auto example = Example<>();
```

So this seems to be related to deduction guides somehow. Could it be that we synthesize a new lambda type for CTAD and happen to mix it with types that we also creates during substitution?

cc @hokein who recently worked on CTAD, in case he has any ideas.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ilya Biryukov (ilya-biryukov)

<details>
The following code fails to compile (see https://godbolt.org/z/zTf49f9oe):
```cpp
template &lt;typename MakeValue = decltype([] { return 1; })&gt;
class Example {
   public:
    using Result = decltype(MakeValue()());
    explicit Example(MakeValue&amp;&amp; make_value = {})
        : make_value_(make_value) {}

    Result Make() { return make_value_(); }

   private:
     MakeValue make_value_;
};

template &lt;typename CastTo = int&gt;
CastTo CastResultTo() {
    auto example = Example();
    return example.Make(); // error: 'Cannot initialize object parameter'
}

int main() { return CastResultTo(); }
```

Adding an empty template argument list makes the error go away (see https://godbolt.org/z/dz7eMaM54):
```cpp
    auto example = Example&lt;&gt;();
```

So this seems to be related to deduction guides somehow. Could it be that we synthesize a new lambda type for CTAD and happen to mix it with types that we also creates during substitution?

cc @<!-- -->hokein who recently worked on CTAD, in case he has any ideas.
</details>


---

