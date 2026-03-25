# Clang segfault parsing yyjson::caster<std::vector<bool>>

**Author:** thirtythreeforty
**URL:** https://github.com/llvm/llvm-project/issues/176308

## Body

LLVM 21.1.8 installed on an Ubuntu 24.04 container, with the upstream LLVM packages:

```
$ apt list llvm-21 clang-21
Listing... Done
clang-21/unknown,now 1:21.1.8~++20251221032922+2078da43e25a-1~exp1~20251221153059.70 amd64 [installed]
llvm-21/unknown,now 1:21.1.8~++20251221032922+2078da43e25a-1~exp1~20251221153059.70 amd64 [installed,automatic]
```

...crashes parsing the attached reproducer (specifically, [this line](https://github.com/yosh-matsuda/cpp-yyjson/blob/864d78c7c2cfa556a79810fa4f4ed11b4778ba6d/include/cpp_yyjson.hpp#L4239) from cpp-yyjson).

```
/usr/bin/clang++-21 -DCPPTRACE_STATIC_DEFINE -DLIBASSERT_STATIC_DEFINE -DLIBASSERT_USE_MAGIC_ENUM -Deigen_assert=LIBASSERT_DEBUG_ASSERT -I/mnt/src/dsp/.. -I/mnt/src/pluglog/. -isystem /mnt/vendor/q/q_lib/include -isystem /mnt/build/_deps/cycfi_infra-src/include -isystem /mnt/build/_deps/cycfi_infra-src/external/filesystem/include -isystem /mnt/build/_deps/cycfi_infra-src/external/optional-lite/include -isystem /mnt/build/_deps/cycfi_infra-src/external/string-view-lite/include -isystem /mnt/vendor/fmt/include -isystem /mnt/vendor/magic_enum/include -isystem /mnt/vendor/eigen -isystem /mnt/build/vendor/libassert/include -isystem /mnt/vendor/libassert/include -isystem /mnt/vendor/cpptrace/include -isystem /mnt/build/vendor/cpptrace/include -isystem /mnt/vendor/FFTConvolver/include -isystem /mnt/vendor/cpp-yyjson/include -isystem /mnt/vendor/yyjson/src -isystem /mnt/vendor/nameof/include -fcolor-diagnostics -O2 -g -DNDEBUG -std=gnu++20 -fPIC -fcolor-diagnostics -mavx2 -mfma -include libassert/assert.hpp -Winvalid-pch -Xclang -include-pch -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx -MD -MT src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -MF src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o.d -o src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -c /mnt/src/dsp/mod_matrix_patch.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/clang++-21 -DCPPTRACE_STATIC_DEFINE -DLIBASSERT_STATIC_DEFINE -DLIBASSERT_USE_MAGIC_ENUM -Deigen_assert=LIBASSERT_DEBUG_ASSERT -I/mnt/src/dsp/.. -I/mnt/src/pluglog/. -isystem /mnt/vendor/q/q_lib/include -isystem /mnt/build/_deps/cycfi_infra-src/include -isystem /mnt/build/_deps/cycfi_infra-src/external/filesystem/include -isystem /mnt/build/_deps/cycfi_infra-src/external/optional-lite/include -isystem /mnt/build/_deps/cycfi_infra-src/external/string-view-lite/include -isystem /mnt/vendor/fmt/include -isystem /mnt/vendor/magic_enum/include -isystem /mnt/vendor/eigen -isystem /mnt/build/vendor/libassert/include -isystem /mnt/vendor/libassert/include -isystem /mnt/vendor/cpptrace/include -isystem /mnt/build/vendor/cpptrace/include -isystem /mnt/vendor/FFTConvolver/include -isystem /mnt/vendor/cpp-yyjson/include -isystem /mnt/vendor/yyjson/src -isystem /mnt/vendor/nameof/include -fcolor-diagnostics -O2 -g -DNDEBUG -std=gnu++20 -fPIC -fcolor-diagnostics -mavx2 -mfma -include libassert/assert.hpp -Winvalid-pch -Xclang -include-pch -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx -MD -MT src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -MF src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o.d -o src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -c /mnt/src/dsp/mod_matrix_patch.cpp
1.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4239:49: current parser token ')'
2.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:23:1: parsing namespace 'yyjson'
3.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4234:5: parsing struct/union/class body 'yyjson::caster<std::vector<bool>>'
4.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4238:9: parsing function body 'yyjson::caster<std::vector<bool>>::to_json'
5.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4238:9: in compound statement ('{}')
#0 0x00007f56095ec91f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
#1 0x00007f56095ea1c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
#2 0x00007f5609523e87 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433e87)
#3 0x00007f5604b83330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
#4 0x00007f560e34f584 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xd9b584)
clang++-21: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/mod_matrix_patch-81e4e0.cpp
clang++-21: note: diagnostic msg: /tmp/mod_matrix_patch-81e4e0.sh
clang++-21: note: diagnostic msg: 

********************
```

[mod_matrix_patch-81e4e0.cpp](https://github.com/user-attachments/files/24658668/mod_matrix_patch-81e4e0.cpp)
[mod_matrix_patch-81e4e0.sh](https://github.com/user-attachments/files/24658669/mod_matrix_patch-81e4e0.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: George Hilliard (thirtythreeforty)

<details>
LLVM 21.1.8 installed on an Ubuntu 24.04 container, with the upstream LLVM packages:

```
$ apt list llvm-21 clang-21
Listing... Done
clang-21/unknown,now 1:21.1.8~++20251221032922+2078da43e25a-1~exp1~20251221153059.70 amd64 [installed]
llvm-21/unknown,now 1:21.1.8~++20251221032922+2078da43e25a-1~exp1~20251221153059.70 amd64 [installed,automatic]
```

...crashes parsing the attached reproducer (specifically, [this line](https://github.com/yosh-matsuda/cpp-yyjson/blob/864d78c7c2cfa556a79810fa4f4ed11b4778ba6d/include/cpp_yyjson.hpp#L4239) from cpp-yyjson).

```
/usr/bin/clang++-21 -DCPPTRACE_STATIC_DEFINE -DLIBASSERT_STATIC_DEFINE -DLIBASSERT_USE_MAGIC_ENUM -Deigen_assert=LIBASSERT_DEBUG_ASSERT -I/mnt/src/dsp/.. -I/mnt/src/pluglog/. -isystem /mnt/vendor/q/q_lib/include -isystem /mnt/build/_deps/cycfi_infra-src/include -isystem /mnt/build/_deps/cycfi_infra-src/external/filesystem/include -isystem /mnt/build/_deps/cycfi_infra-src/external/optional-lite/include -isystem /mnt/build/_deps/cycfi_infra-src/external/string-view-lite/include -isystem /mnt/vendor/fmt/include -isystem /mnt/vendor/magic_enum/include -isystem /mnt/vendor/eigen -isystem /mnt/build/vendor/libassert/include -isystem /mnt/vendor/libassert/include -isystem /mnt/vendor/cpptrace/include -isystem /mnt/build/vendor/cpptrace/include -isystem /mnt/vendor/FFTConvolver/include -isystem /mnt/vendor/cpp-yyjson/include -isystem /mnt/vendor/yyjson/src -isystem /mnt/vendor/nameof/include -fcolor-diagnostics -O2 -g -DNDEBUG -std=gnu++20 -fPIC -fcolor-diagnostics -mavx2 -mfma -include libassert/assert.hpp -Winvalid-pch -Xclang -include-pch -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx -MD -MT src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -MF src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o.d -o src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -c /mnt/src/dsp/mod_matrix_patch.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/clang++-21 -DCPPTRACE_STATIC_DEFINE -DLIBASSERT_STATIC_DEFINE -DLIBASSERT_USE_MAGIC_ENUM -Deigen_assert=LIBASSERT_DEBUG_ASSERT -I/mnt/src/dsp/.. -I/mnt/src/pluglog/. -isystem /mnt/vendor/q/q_lib/include -isystem /mnt/build/_deps/cycfi_infra-src/include -isystem /mnt/build/_deps/cycfi_infra-src/external/filesystem/include -isystem /mnt/build/_deps/cycfi_infra-src/external/optional-lite/include -isystem /mnt/build/_deps/cycfi_infra-src/external/string-view-lite/include -isystem /mnt/vendor/fmt/include -isystem /mnt/vendor/magic_enum/include -isystem /mnt/vendor/eigen -isystem /mnt/build/vendor/libassert/include -isystem /mnt/vendor/libassert/include -isystem /mnt/vendor/cpptrace/include -isystem /mnt/build/vendor/cpptrace/include -isystem /mnt/vendor/FFTConvolver/include -isystem /mnt/vendor/cpp-yyjson/include -isystem /mnt/vendor/yyjson/src -isystem /mnt/vendor/nameof/include -fcolor-diagnostics -O2 -g -DNDEBUG -std=gnu++20 -fPIC -fcolor-diagnostics -mavx2 -mfma -include libassert/assert.hpp -Winvalid-pch -Xclang -include-pch -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /mnt/build/src/dsp/CMakeFiles/metastring_dsp.dir/cmake_pch.hxx -MD -MT src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -MF src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o.d -o src/dsp/CMakeFiles/metastring_dsp.dir/mod_matrix_patch.cpp.o -c /mnt/src/dsp/mod_matrix_patch.cpp
1.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4239:49: current parser token ')'
2.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:23:1: parsing namespace 'yyjson'
3.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4234:5: parsing struct/union/class body 'yyjson::caster&lt;std::vector&lt;bool&gt;&gt;'
4.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4238:9: parsing function body 'yyjson::caster&lt;std::vector&lt;bool&gt;&gt;::to_json'
5.      /mnt/vendor/cpp-yyjson/include/cpp_yyjson.hpp:4238:9: in compound statement ('{}')
#<!-- -->0 0x00007f56095ec91f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
#<!-- -->1 0x00007f56095ea1c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
#<!-- -->2 0x00007f5609523e87 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433e87)
#<!-- -->3 0x00007f5604b83330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
#<!-- -->4 0x00007f560e34f584 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xd9b584)
clang++-21: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/mod_matrix_patch-81e4e0.cpp
clang++-21: note: diagnostic msg: /tmp/mod_matrix_patch-81e4e0.sh
clang++-21: note: diagnostic msg: 

********************
```

[mod_matrix_patch-81e4e0.cpp](https://github.com/user-attachments/files/24658668/mod_matrix_patch-81e4e0.cpp)
[mod_matrix_patch-81e4e0.sh](https://github.com/user-attachments/files/24658669/mod_matrix_patch-81e4e0.sh)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try `main` branch? https://godbolt.org should be helpful. 21 is not longer supported after 22 branch creation.

---

### Comment 3 - thirtythreeforty

Clang 22 from snapshot builds (`Ubuntu clang version 22.0.0 (++20260105081943+212527c00ba6-1~exp1~20260105082109.1379)`) does not crash.

---

