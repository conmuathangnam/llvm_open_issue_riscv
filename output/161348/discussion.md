# [clang] concepts/require crash clang

**Author:** kikaxa
**URL:** https://github.com/llvm/llvm-project/issues/161348

## Body

In frontend, according to the detection guide.

Reproducible on macos and most new/old clang versions i tried on godbolt(18-21), gcc works all right.

[1.cpp](https://github.com/user-attachments/files/22616814/1.cpp)

Compiler stderr
gmake[2]: *** [CMakeFiles/StdAnyTest.dir/build.make:79: CMakeFiles/StdAnyTest.dir/main.cpp.o] Segmentation fault (core dumped)
--
clang: error: unable to execute command: Illegal instruction: 4
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple clang version 16.0.0 (clang-1600.0.26.4)

[1-a445e2.sh](https://github.com/user-attachments/files/22616816/1-a445e2.sh)
[1-a445e2.cpp](https://github.com/user-attachments/files/22616817/1-a445e2.cpp)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch of LLVM, not Apple code?

---

### Comment 2 - kikaxa

>Reproducible on macos and most new/old clang versions i tried on godbolt(18-21)    

x86-64 clang 21.1.0 on goldbolt

---

### Comment 3 - EugeneZelenko

> > Reproducible on macos and most new/old clang versions i tried on godbolt(18-21)
> 
> x86-64 clang 21.1.0 on goldbolt

Please provide relevant link here.

---

### Comment 4 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/n4jM76hvK))
```C++
#include <utility>

template<class X>
void run(const X&&) {}

template<class T = void>
concept RefRunnable = requires {
  [](decltype([]{}) f) -> decltype(::run(std::move(f))) { };
};

static_assert(RefRunnable<void>);
```

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (kikaxa)

<details>
In frontend, according to the detection guide.

Reproducible on macos and most new/old clang versions i tried on godbolt(18-21), gcc works all right.

[1.cpp](https://github.com/user-attachments/files/22616814/1.cpp)

Compiler stderr
gmake[2]: *** [CMakeFiles/StdAnyTest.dir/build.make:79: CMakeFiles/StdAnyTest.dir/main.cpp.o] Segmentation fault (core dumped)
--
clang: error: unable to execute command: Illegal instruction: 4
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple clang version 16.0.0 (clang-1600.0.26.4)

[1-a445e2.sh](https://github.com/user-attachments/files/22616816/1-a445e2.sh)
[1-a445e2.cpp](https://github.com/user-attachments/files/22616817/1-a445e2.cpp)
</details>


---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (kikaxa)

<details>
In frontend, according to the detection guide.

Reproducible on macos and most new/old clang versions i tried on godbolt(18-21), gcc works all right.

[1.cpp](https://github.com/user-attachments/files/22616814/1.cpp)

Compiler stderr
gmake[2]: *** [CMakeFiles/StdAnyTest.dir/build.make:79: CMakeFiles/StdAnyTest.dir/main.cpp.o] Segmentation fault (core dumped)
--
clang: error: unable to execute command: Illegal instruction: 4
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple clang version 16.0.0 (clang-1600.0.26.4)

[1-a445e2.sh](https://github.com/user-attachments/files/22616816/1-a445e2.sh)
[1-a445e2.cpp](https://github.com/user-attachments/files/22616817/1-a445e2.cpp)
</details>


---

### Comment 7 - frederick-vs-ja

This may be a duplicate of #123854 or #70064.

---

### Comment 8 - shafik

This goes back to clang-17 but the other two go back to clang-18.

---

### Comment 9 - shafik

@cor3ntin this bisects to 93d7002dc4644b0a6f15a998dff0d55c72012e87

---

