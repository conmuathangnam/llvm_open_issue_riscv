# friend function with default parameters causes "redefinition of default argument" error in templates

**Author:** wingerZYM
**URL:** https://github.com/llvm/llvm-project/issues/130917
**Status:** Closed
**Labels:** clang:frontend, regression:20
**Closed Date:** 2025-04-14T15:22:36Z

## Body

### Summary:
Starting from commit `38b3d87bd384a469a6618ec6a971352cb4f813ba` in LLVM, Clang 20 reports 
`error: redefinition of default argument` when using `friend` functions 
in template classes, even though the default argument is only defined once.

### Steps to Reproduce:
Compile the following minimal example with Clang 20:

```cpp
#include <iostream>
#include <memory>

namespace Work {

namespace detail {
    class Obj;
} // namespace detail

using Ptr = std::unique_ptr<detail::Obj>;

template <int>
Ptr Create(const void* key = nullptr);  // default argument only once.

namespace detail {

class Obj {
protected:
    Obj(const void* key) {
        std::cout << "Obj::Obj\n";
    }
};

template <int>
class ObjImpl : public Obj {
public:
    ObjImpl(const void* key)
        : Obj(key) {
        std::cout << "ObjImpl::ObjImpl\n";
    }
    template <int>
    friend Ptr Work::Create(const void*);
};

} // namespace detail

template <int i>
Ptr Create(const void* key) {  // error: redefinition of default argument
    return Ptr(new detail::ObjImpl<i>(key));
}

} // namespace Work

int main() {
    auto ptr = Work::Create<192>(nullptr);
    return 0;
}
```
### Result:
clang++ reports:
```
./build/bin/clang++ -std=c++20 /mnt/e/test/error.cpp
/mnt/e/test/error.cpp:38:24: error: redefinition of default argument
   13 | Ptr Create(const void* key) {
      |                        ^
/mnt/e/test/error.cpp:39:20: note: in instantiation of template class 'Work::detail::ObjImpl<192>' requested here
   39 |     return Ptr(new detail::ObjImpl<i>(key));
      |                    ^
/mnt/e/test/error.cpp:45:22: note: in instantiation of function template specialization 'Work::Create<192>' requested here
   45 |     auto ptr = Work::Create<192>(nullptr);
      |                      ^
/mnt/e/test/error.cpp:32:40: note: previous definition is here
   32 |     friend Ptr Work::Create(const void*);
      |                                        ^
1 error generated.
```
### Workaround:
If we remove the default parameter from Ptr Create(const void* key = nullptr);, then Clang++ compiles correctly.
If we remove the friend declaration, Clang++ also compiles correctly.
### Regression:
git branch: release/20.x
First bad commit: 38b3d87bd384a469a6618ec6a971352cb4f813ba
This issue does not occur in earlier versions of branch.

## Comments

### Comment 1 - frederick-vs-ja

Slightly reduced example ([Godbolt link](https://godbolt.org/z/9TWG94GT9)):
```C++
template <int>
void Create(const void* = nullptr);

template <int>
struct ObjImpl {
  template <int>
  friend void ::Create(const void*);
};

template <int I>
void Create(const void*) {
  (void) ObjImpl<I>{};
}

int main() {
  Create<42>();
}
```

This is probably a regression since Clang 20.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wingerZYM)

<details>
### Summary:
Starting from commit `38b3d87bd384a469a6618ec6a971352cb4f813ba` in LLVM, Clang 20 reports 
`error: redefinition of default argument` when using `friend` functions 
in template classes, even though the default argument is only defined once.

### Steps to Reproduce:
Compile the following minimal example with Clang 20:

```cpp
#include &lt;iostream&gt;
#include &lt;memory&gt;

namespace Work {

namespace detail {
    class Obj;
} // namespace detail

using Ptr = std::unique_ptr&lt;detail::Obj&gt;;

template &lt;int&gt;
Ptr Create(const void* key = nullptr);  // default argument only once.

namespace detail {

class Obj {
protected:
    Obj(const void* key) {
        std::cout &lt;&lt; "Obj::Obj\n";
    }
};

template &lt;int&gt;
class ObjImpl : public Obj {
public:
    ObjImpl(const void* key)
        : Obj(key) {
        std::cout &lt;&lt; "ObjImpl::ObjImpl\n";
    }
    template &lt;int&gt;
    friend Ptr Work::Create(const void*);
};

} // namespace detail

template &lt;int i&gt;
Ptr Create(const void* key) {  // error: redefinition of default argument
    return Ptr(new detail::ObjImpl&lt;i&gt;(key));
}

} // namespace Work

int main() {
    auto ptr = Work::Create&lt;192&gt;(nullptr);
    return 0;
}
```
### Result:
clang++ reports:
```
./build/bin/clang++ -std=c++20 /mnt/e/test/error.cpp
/mnt/e/test/error.cpp:38:24: error: redefinition of default argument
   13 | Ptr Create(const void* key) {
      |                        ^
/mnt/e/test/error.cpp:39:20: note: in instantiation of template class 'Work::detail::ObjImpl&lt;192&gt;' requested here
   39 |     return Ptr(new detail::ObjImpl&lt;i&gt;(key));
      |                    ^
/mnt/e/test/error.cpp:45:22: note: in instantiation of function template specialization 'Work::Create&lt;192&gt;' requested here
   45 |     auto ptr = Work::Create&lt;192&gt;(nullptr);
      |                      ^
/mnt/e/test/error.cpp:32:40: note: previous definition is here
   32 |     friend Ptr Work::Create(const void*);
      |                                        ^
1 error generated.
```
### Workaround:
If we remove the default parameter from Ptr Create(const void* key = nullptr);, then Clang++ compiles correctly.
If we remove the friend declaration, Clang++ also compiles correctly.
### Regression:
git branch: release/20.x
First bad commit: 38b3d87bd384a469a6618ec6a971352cb4f813ba
This issue does not occur in earlier versions of branch.
</details>


---

### Comment 3 - zyn0217

@mizvekov I suspect https://github.com/llvm/llvm-project/pull/125266 might be related, but I'm not quite sure

---

### Comment 4 - mizvekov

If you look at commit which the author pointed out, it's pretty suspect of this problem: https://github.com/llvm/llvm-project/pull/111992

It has changes to template instantiation friend definition tracking, outside of the serialization / modules area, which look suspect and would need careful review.

CC @dmpolukhin @ChuanqiXu9 as authors and reviewer of the original commit.

---

### Comment 5 - zyn0217

> If you look at commit which the author pointed out, it's pretty suspect of this problem: https://github.com/llvm/llvm-project/pull/111992
> 

Oops, sorry I didn't even notice that line!



---

### Comment 6 - dmpolukhin

I'll take a look what is happening with this example and how to fix it. Unfortunately I don't know the way how to distinguish real redeclarations from artificial redeclaration that happens on AST merges.

---

### Comment 7 - dmpolukhin

Should I merges this fix to clang-20?

---

### Comment 8 - zyn0217

> Should I merges this fix to clang-20?

I think so, yes

---

### Comment 9 - dmpolukhin

Fix cannot be cherry-picked to clang-20, see discussion in #134232 so closing this bug.

---

