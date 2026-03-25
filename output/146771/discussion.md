# [C++20][Modules][ABI] Silent bad codegen in module with TU-local exposure

**Author:** barcharcraz
**URL:** https://github.com/llvm/llvm-project/issues/146771
**Status:** Closed
**Labels:** duplicate, clang:modules, ABI
**Closed Date:** 2025-07-03T02:06:23Z

## Body

consider:

mod1.cppm
```c++
export module mod1;

static int func1_internal() { return 4;}
export inline int func2_external() { return func1_internal(); }
```

main.cpp
```c++
import mod1;
static int func1_internal() {
        return 5;
}
int main() {
        return func2_external() + func1_internal();
}
```

this program returns "8" when compiled with clang.

This code is ill-formed, because `func2_external()` exposes the TU-local `func1_internal()`, but clang doesn't diagnose it.

Instead of diagnosing clang instead generates bad code, emitting code for the definition of func1_internal inside mod1.cppm when compiling the main.cpp TU and emitting no code for "func1_internal()" inside main.

Sidenote: right now it seems like just adding inline is enough to cause func2_external() to be emitted as a weak symbol in importing TUs, which isn't really ideal, it could just as well still be a strong extern symbol emitted inside mod1's associated object file.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Charlie Barto (barcharcraz)

<details>
consider:

mod1.cppm
```c++
export module mod1;

static int func1_internal() { return 4;}
export inline int func2_external() { return func1_internal(); }
```

main.cpp
```c++
import mod1;
static int func1_internal() {
        return 5;
}
int main() {
        return func2_external() + func1_internal();
}
```

this program returns "8" when compiled with clang.

This code is ill-formed, because `func2_external()` exposes the TU-local `func1_internal()`, but clang doesn't diagnose it.

Instead of diagnosing clang instead generates bad code, emitting code for the definition of func1_internal inside mod1.cppm when compiling the main.cpp TU and emitting no code for "func1_internal()" inside main.

Sidenote: right now it seems like just adding inline is enough to cause func2_external() to be emitted as a weak symbol in importing TUs, which isn't really ideal, it could just as well still be a strong extern symbol emitted inside mod1's associated object file.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Charlie Barto (barcharcraz)

<details>
consider:

mod1.cppm
```c++
export module mod1;

static int func1_internal() { return 4;}
export inline int func2_external() { return func1_internal(); }
```

main.cpp
```c++
import mod1;
static int func1_internal() {
        return 5;
}
int main() {
        return func2_external() + func1_internal();
}
```

this program returns "8" when compiled with clang.

This code is ill-formed, because `func2_external()` exposes the TU-local `func1_internal()`, but clang doesn't diagnose it.

Instead of diagnosing clang instead generates bad code, emitting code for the definition of func1_internal inside mod1.cppm when compiling the main.cpp TU and emitting no code for "func1_internal()" inside main.

Sidenote: right now it seems like just adding inline is enough to cause func2_external() to be emitted as a weak symbol in importing TUs, which isn't really ideal, it could just as well still be a strong extern symbol emitted inside mod1's associated object file.
</details>


---

### Comment 3 - cor3ntin

@ChuanqiXu9 

---

### Comment 4 - ChuanqiXu9

Duplicate with https://github.com/llvm/llvm-project/issues/112294

---

