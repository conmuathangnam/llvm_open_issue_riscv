# [clang-cl] access violation with an extern reference at runtime with clang-cl 20.1.0-rc2 using /std:c++latest

**Author:** Mishura4
**URL:** https://github.com/llvm/llvm-project/issues/127572
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, miscompilation
**Closed Date:** 2025-03-23T02:52:14Z

## Body

```c++
// test.hpp

#include <vector>
#include <string>

extern const std::vector<std::string>& vec_ref;

void init();
```

```c++
// test.cpp
#include "test.hpp"

namespace
{

std::vector<std::string> impl;

}

const std::vector<std::string>& vec_ref = impl;

void init() {
    std::vector<std::string> vec;

    vec.push_back("hello");
    vec.push_back("world!");

    impl = std::move(vec);
}
```

```c++
// main.cpp
#include "test.hpp"

#include <iostream>

int main()
{
    init();
    for (auto& str : vec_ref)
    {
        std::cout << str << std::endl;
    }
}
```

Compile with `clang-cl test.cpp main.cpp /std:c++latest`. For some reason either `vec_ref` or `impl` seems to become corrupted and the for loop seems to read garbage data. In the project I initially encountered this, the program crashed with an access violation in std::vector::begin().

Observations:
- The issue seems to be with `/std:c++latest`, the same code works without an issue on 20.1.0-rc2 with `/std:c++20`
- The same code works without an issue on 19.1.7 even with `/std:c++latest`.

clang-cl was compiled with:
```bat
set CXX=clang-cl
set CC=clang-cl
set VECTORIZE="-march=native -mtune=native"

cmake llvm/ -B build ^
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;lldb" ^
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx" ^
  -DLLVM_TARGETS_TO_BUILD=Native ^
  -DLLVM_PARALLEL_COMPILE_JOBS=30 ^
  -DLLVM_PARALLEL_LINK_JOBS=3 ^
  -DLLVM_ENABLE_LLD=on ^
  -DCLANG_DEFAULT_LINKER=lld ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCLANG_ENABLE_BOOTSTRAP=On ^
  -DLLVM_HOST_TRIPLE=x86_64-pc-windows-msvc ^
  -DLLVM_ENABLE_LTO=Thin ^
  -DLLVM_ENABLE_EH=ON ^
  -DLLVM_ENABLE_RTTI=ON ^
  -DLLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_CXX_STANDARD=20 ^
  -DBOOTSTRAP_CMAKE_CXX_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_C_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_EXE_LINKER_FLAGS=%VECTORIZE% ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_EH=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_RTTI=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_MT=mt.exe ^
  -G Ninja

ninja -C build
```

(Issue also happens with `-fuse-ld=link`)

## Comments

### Comment 1 - dtcxzyw

It may be duplicate of https://github.com/llvm/llvm-project/issues/127475


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Miuna (Mishura4)

<details>
```c++
// test.hpp

#include &lt;vector&gt;
#include &lt;string&gt;

extern const std::vector&lt;std::string&gt;&amp; vec_ref;

void init();
```

```c++
// test.cpp
#include "test.hpp"

namespace
{

std::vector&lt;std::string&gt; impl;

}

const std::vector&lt;std::string&gt;&amp; vec_ref = impl;

void init() {
    std::vector&lt;std::string&gt; vec;

    vec.push_back("hello");
    vec.push_back("world!");

    impl = std::move(vec);
}
```

```c++
// main.cpp
#include "test.hpp"

#include &lt;iostream&gt;

int main()
{
    init();
    for (auto&amp; str : vec_ref)
    {
        std::cout &lt;&lt; str &lt;&lt; std::endl;
    }
}
```

Compile with `clang-cl test.cpp main.cpp /std:c++latest`. For some reason either `vec_ref` or `impl` seems to become corrupted and the for loop seems to read garbage data. In the project I initially encountered this, the program crashed with an access violation in std::vector::begin().

Observations:
- The issue seems to be with `/std:c++latest`, the same code works without an issue on 20.1.0-rc2 with `/std:c++20`
- The same code works without an issue on 19.1.7 even with `/std:c++latest`.

clang-cl was compiled with:
```bat
set CXX=clang-cl
set CC=clang-cl
set VECTORIZE="-march=native -mtune=native"

cmake llvm/ -B build ^
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;lldb" ^
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx" ^
  -DLLVM_TARGETS_TO_BUILD=Native ^
  -DLLVM_PARALLEL_COMPILE_JOBS=30 ^
  -DLLVM_PARALLEL_LINK_JOBS=3 ^
  -DLLVM_ENABLE_LLD=on ^
  -DCLANG_DEFAULT_LINKER=lld ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCLANG_ENABLE_BOOTSTRAP=On ^
  -DLLVM_HOST_TRIPLE=x86_64-pc-windows-msvc ^
  -DLLVM_ENABLE_LTO=Thin ^
  -DLLVM_ENABLE_EH=ON ^
  -DLLVM_ENABLE_RTTI=ON ^
  -DLLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_CXX_STANDARD=20 ^
  -DBOOTSTRAP_CMAKE_CXX_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_C_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_EXE_LINKER_FLAGS=%VECTORIZE% ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_EH=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_RTTI=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_MT=mt.exe ^
  -G Ninja

ninja -C build
```

(Issue also happens with `-fuse-ld=link`)
</details>


---

### Comment 3 - Mishura4

Yep, looks very similar

---

### Comment 4 - Mishura4

Fixed in 9010db1b84ef81d23266b5c57569beadd7967ddb

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Miuna (Mishura4)

<details>
```c++
// test.hpp

#include &lt;vector&gt;
#include &lt;string&gt;

extern const std::vector&lt;std::string&gt;&amp; vec_ref;

void init();
```

```c++
// test.cpp
#include "test.hpp"

namespace
{

std::vector&lt;std::string&gt; impl;

}

const std::vector&lt;std::string&gt;&amp; vec_ref = impl;

void init() {
    std::vector&lt;std::string&gt; vec;

    vec.push_back("hello");
    vec.push_back("world!");

    impl = std::move(vec);
}
```

```c++
// main.cpp
#include "test.hpp"

#include &lt;iostream&gt;

int main()
{
    init();
    for (auto&amp; str : vec_ref)
    {
        std::cout &lt;&lt; str &lt;&lt; std::endl;
    }
}
```

Compile with `clang-cl test.cpp main.cpp /std:c++latest`. For some reason either `vec_ref` or `impl` seems to become corrupted and the for loop seems to read garbage data. In the project I initially encountered this, the program crashed with an access violation in std::vector::begin().

Observations:
- The issue seems to be with `/std:c++latest`, the same code works without an issue on 20.1.0-rc2 with `/std:c++20`
- The same code works without an issue on 19.1.7 even with `/std:c++latest`.

clang-cl was compiled with:
```bat
set CXX=clang-cl
set CC=clang-cl
set VECTORIZE="-march=native -mtune=native"

cmake llvm/ -B build ^
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;lldb" ^
  -DLLVM_ENABLE_RUNTIMES="compiler-rt;libcxx" ^
  -DLLVM_TARGETS_TO_BUILD=Native ^
  -DLLVM_PARALLEL_COMPILE_JOBS=30 ^
  -DLLVM_PARALLEL_LINK_JOBS=3 ^
  -DLLVM_ENABLE_LLD=on ^
  -DCLANG_DEFAULT_LINKER=lld ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCLANG_ENABLE_BOOTSTRAP=On ^
  -DLLVM_HOST_TRIPLE=x86_64-pc-windows-msvc ^
  -DLLVM_ENABLE_LTO=Thin ^
  -DLLVM_ENABLE_EH=ON ^
  -DLLVM_ENABLE_RTTI=ON ^
  -DLLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_CXX_STANDARD=20 ^
  -DBOOTSTRAP_CMAKE_CXX_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_C_FLAGS=%VECTORIZE% ^
  -DBOOTSTRAP_CMAKE_EXE_LINKER_FLAGS=%VECTORIZE% ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_EH=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_RTTI=ON ^
  -DRUNTIMES_x86_64-pc-windows-msvc_LLVM_ENABLE_LIBXML2=ON ^
  -DCMAKE_MT=mt.exe ^
  -G Ninja

ninja -C build
```

(Issue also happens with `-fuse-ld=link`)
</details>


---

