# [clang] RISC-V: failed to perform tail call elimination on a call site marked musttail

**Author:** xypron
**URL:** https://github.com/llvm/llvm-project/issues/157814

## Body

I originally experienced this error when compiling WebKit with gcc https://gcc.gnu.org/bugzilla/show_bug.cgi?id=121784. But it is also reproducible in LLVM:

Compiling the following program with clang results in an error "failed to perform tail call elimination on a call site marked musttail".
```
struct StageList;
using F = __attribute__((vector_size(16))) float;
using StageFn = void (*)(StageList list, int src, int dst, F r, F g, F b);
struct StageList {
  const StageFn* fn;
};

void m(StageList list, int src, int dst,  F r, F g, F b) {
  [[clang::musttail]] 
 return (*list.fn)(list, src, dst, r, g, b);
} 
```
My expectation is that tail calls should be usable with any number of arguments provided that they are trivially destructible.

[test-a79310.sh](https://github.com/user-attachments/files/22251361/test-a79310.sh)
[test-a79310.cpp](https://github.com/user-attachments/files/22251364/test-a79310.cpp)
[clang.log](https://github.com/user-attachments/files/22251372/clang.log)

## Comments

### Comment 1 - frederick-vs-ja

Clang is crashing for this example. [Godbolt link](https://godbolt.org/z/vYj854Te6).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Heinrich Schuchardt (xypron)

<details>
I originally experienced this error when compiling WebKit with gcc https://gcc.gnu.org/bugzilla/show_bug.cgi?id=121784. But it is also reproducible in LLVM:

Compiling the following program with clang results in an error "failed to perform tail call elimination on a call site marked musttail".
```
struct StageList;
using F = __attribute__((vector_size(16))) float;
using StageFn = void (*)(StageList list, int src, int dst, F r, F g, F b);
struct StageList {
  const StageFn* fn;
};

void m(StageList list, int src, int dst,  F r, F g, F b) {
  [[clang::musttail]] 
 return (*list.fn)(list, src, dst, r, g, b);
} 
```
My expectation is that tail calls should be usable with any number of arguments provided that they are trivially destructible.

[test-a79310.sh](https://github.com/user-attachments/files/22251361/test-a79310.sh)
[test-a79310.cpp](https://github.com/user-attachments/files/22251364/test-a79310.cpp)
[clang.log](https://github.com/user-attachments/files/22251372/clang.log)
</details>


---

### Comment 3 - frederick-vs-ja

This is possibly a duplicate. See #56908 and #84090.

---

