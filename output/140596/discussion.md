# Aggregate deduction broken in function templates

**Author:** snarkmaster
**URL:** https://github.com/llvm/llvm-project/issues/140596

## Body

The following snippet works on MSVC and GCC, but fails to compile `fnTemplate` under Clang -- https://godbolt.org/z/KvjTqarxa:

```cpp
struct Inner {
  // Clang works if you comment this
  Inner() = default;
};

template <typename T>
struct WantsCTAD {
  T t_;
};

auto fn() {
    WantsCTAD ok{.t_ = Inner{}};
    (void)ok;
}

template <typename>
void fnTemplate() {
    // error: no viable constructor or deduction guide for deduction of template arguments of 'WantsCTAD')
    WantsCTAD bad{.t_ = Inner{}};
    (void)bad;
}

int main() {
  fn();
  fnTemplate<int>();
  return 0;
}
```

I'm a bad language lawyer, so I can't say with confidence whether Clang or GCC/MSVC are out of compliance here, but ... it's surprising and inconsistent that this behavior is contingent on both of these:
 - asking for CTAD in a function template (rather than just function)
 - adding a user-declared ctor to `Inner` -- IIUC as of C++20 this makes `Inner` not an aggregate type


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (snarkmaster)

<details>
The following snippet works on MSVC and GCC, but fails to compile `fnTemplate` under Clang -- https://godbolt.org/z/KvjTqarxa:

```cpp
struct Inner {
  // Clang works if you comment this
  Inner() = default;
};

template &lt;typename T&gt;
struct WantsCTAD {
  T t_;
};

auto fn() {
    WantsCTAD ok{.t_ = Inner{}};
    (void)ok;
}

template &lt;typename&gt;
void fnTemplate() {
    // error: no viable constructor or deduction guide for deduction of template arguments of 'WantsCTAD')
    WantsCTAD bad{.t_ = Inner{}};
    (void)bad;
}

int main() {
  fn();
  fnTemplate&lt;int&gt;();
  return 0;
}
```

I'm a bad language lawyer, so I can't say with confidence whether Clang or GCC/MSVC are out of compliance here, but ... it's surprising and inconsistent that this behavior is contingent on both of these:
 - asking for CTAD in a function template (rather than just function)
 - adding a user-declared ctor to `Inner` -- IIUC as of C++20 this makes `Inner` not an aggregate type

</details>


---

### Comment 2 - frederick-vs-ja

Whether `Inner` is an aggregate type is unrelated.

Confirmed on the trunk ([Godbolt link](https://godbolt.org/z/h6xcGoeWE)).

---

### Comment 3 - cor3ntin

@hokein @antangelo 

---

### Comment 4 - MagentaTreehouse

Maybe related: #99845

---

