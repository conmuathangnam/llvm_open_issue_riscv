# clang: pack expansion contains parameter pack 'Args' that has a different length (1 vs. 2) from outer parameter packs

**Author:** jcelerier
**URL:** https://github.com/llvm/llvm-project/issues/64902
**Status:** Closed
**Labels:** clang:frontend, rejects-valid
**Closed Date:** 2024-12-19T05:06:16Z

## Body

Hello,

repro:

```c++
#include <functional>

struct worker {
    std::function<void(int, int)> request;
    static void work(int, int);
} worker;

template <typename T = int>
void connect_worker() {
    worker.request = []<typename... Args>(Args&&... f) {
        [... ff = std::forward<Args>(f)]() mutable {
            worker::work(std::forward<Args>(ff)...);
        }();
    };
}

int main() { connect_worker(); }
```

causes: 

```console
<source>:14:48: error: pack expansion contains parameter pack 'Args' that has a different length (1 vs. 2) from outer parameter packs
            worker::work(std::forward<Args>(ff)...);
                                      ~~~~     ^
```

I'm confident this is a compiler bug as if one turns `connect_worker` into a non-template by removing `
template <typename T = int>`, then the code builds and runs as expected:

https://gcc.godbolt.org/z/YM6ar7WG3

also,

```cpp
static_assert(sizeof...(Args) == 2);
```

Just before the offending line passes too.

Not forwarding the arguments works too: 

```cpp
worker::work(ff...);
```

## Comments

### Comment 1 - shafik

Maybe duplicate of: https://github.com/llvm/llvm-project/issues/61415

CC @erichkeane 

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - zyn0217

Should have been fixed by #101385

---

