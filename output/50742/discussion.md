# -finstrument-functions + libstdc++ + C++17/20 = undefined symbol linker error

**Author:** fc2ef0fe-53df-4235-b4bb-a3668189de68
**URL:** https://github.com/llvm/llvm-project/issues/50742
**Status:** Closed
**Labels:** c++20, bugzilla, miscompilation, llvm:transforms
**Closed Date:** 2025-01-03T17:25:09Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [51400](https://llvm.org/bz51400) |
| Version | trunk |
| OS | Linux |
| Attachments | [MRE and test script](https://user-images.githubusercontent.com/93435533/143762403-3b781c0b-f30a-4bed-bffc-f44270f64496.gz) |
| CC | @mattfbacon,@zygoloid |

## Extended Description 


When compiling with `-finstrument-functions` and any standard above C++17, that is C++2a aka 20 or 2b aka 23, when attempting to declare a string, the proper templates relating to standard library allocators are not included in the object files, leading to the following linker error:

```
/usr/bin/ld: /tmp/main-6f2635.o: in function `std::allocator_traits<std::allocator<char> >::deallocate(std::allocator<char>&, char*, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x5d): undefined reference to `std::allocator<char>::deallocate(char*, unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x99): undefined reference to `std::allocator<char>::deallocate(char*, unsigned long)'
clang-14: error: linker command failed with exit code 1 (use -v to see invocation)
```

Additionally, if the string has contents, the error will include the inability to find an allocate method:

```
/usr/bin/ld: /tmp/main-248bd2.o: in function `std::allocator_traits<std::allocator<char> >::deallocate(std::allocator<char>&, char*, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x5d): undefined reference to `std::allocator<char>::deallocate(char*, unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x99): undefined reference to `std::allocator<char>::deallocate(char*, unsigned long)'
/usr/bin/ld: /tmp/main-248bd2.o: in function `std::allocator_traits<std::allocator<char> >::allocate(std::allocator<char>&, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE8allocateERS0_m[_ZNSt16allocator_traitsISaIcEE8allocateERS0_m]+0x49): undefined reference to `std::allocator<char>::allocate(unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE8allocateERS0_m[_ZNSt16allocator_traitsISaIcEE8allocateERS0_m]+0x81): undefined reference to `std::allocator<char>::allocate(unsigned long)'
clang-14: error: linker command failed with exit code 1 (use -v to see invocation)
```

This manifests for `-std=c++20`, `-std=gnu++20`, `-std=c++2b`, `-std=gnu++2b`, but not for `std=c++17`. Additionally, the g++ can compile the code with the same flags, for every C++ standard I tested, from c++17 to c++2b.

See attached tarball for a minimal reproducible example and a script to test whether clang++ and g++ can build it for each standard.

I wasn't sure if this qualifies as release blocker because theoretically you could remove `-finstrument-functions`, in which case it compiles just fine. However I like seeing all the function names in debugging tools like ASAN.

## Comments

### Comment 1 - fc2ef0fe-53df-4235-b4bb-a3668189de68

Godbolt demo link in case you don't want to download the gzip: https://godbolt.org/z/19j1MTa9j

---

### Comment 2 - frederick-vs-ja

When I added `template class std::allocator<char>;` the program (to request explicit instantiation of non-template member functions of that type), linking was successful, but this shouldn't be needed.

Since C++20, many related functions are made `constexpr`, and inlining become more aggressive. I think Clang mistakenly ignored requests for binary function bodies needed for `-finstrument-functions`.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (fc2ef0fe-53df-4235-b4bb-a3668189de68)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [51400](https://llvm.org/bz51400) |
| Version | trunk |
| OS | Linux |
| Attachments | [MRE and test script](https://user-images.githubusercontent.com/93435533/143762403-3b781c0b-f30a-4bed-bffc-f44270f64496.gz) |
| CC | @<!-- -->mattfbacon,@<!-- -->zygoloid |

## Extended Description 


When compiling with `-finstrument-functions` and any standard above C++17, that is C++2a aka 20 or 2b aka 23, when attempting to declare a string, the proper templates relating to standard library allocators are not included in the object files, leading to the following linker error:

```
/usr/bin/ld: /tmp/main-6f2635.o: in function `std::allocator_traits&lt;std::allocator&lt;char&gt; &gt;::deallocate(std::allocator&lt;char&gt;&amp;, char*, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x5d): undefined reference to `std::allocator&lt;char&gt;::deallocate(char*, unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x99): undefined reference to `std::allocator&lt;char&gt;::deallocate(char*, unsigned long)'
clang-14: error: linker command failed with exit code 1 (use -v to see invocation)
```

Additionally, if the string has contents, the error will include the inability to find an allocate method:

```
/usr/bin/ld: /tmp/main-248bd2.o: in function `std::allocator_traits&lt;std::allocator&lt;char&gt; &gt;::deallocate(std::allocator&lt;char&gt;&amp;, char*, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x5d): undefined reference to `std::allocator&lt;char&gt;::deallocate(char*, unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm[_ZNSt16allocator_traitsISaIcEE10deallocateERS0_Pcm]+0x99): undefined reference to `std::allocator&lt;char&gt;::deallocate(char*, unsigned long)'
/usr/bin/ld: /tmp/main-248bd2.o: in function `std::allocator_traits&lt;std::allocator&lt;char&gt; &gt;::allocate(std::allocator&lt;char&gt;&amp;, unsigned long)':
main.cpp:(.text._ZNSt16allocator_traitsISaIcEE8allocateERS0_m[_ZNSt16allocator_traitsISaIcEE8allocateERS0_m]+0x49): undefined reference to `std::allocator&lt;char&gt;::allocate(unsigned long)'
/usr/bin/ld: main.cpp:(.text._ZNSt16allocator_traitsISaIcEE8allocateERS0_m[_ZNSt16allocator_traitsISaIcEE8allocateERS0_m]+0x81): undefined reference to `std::allocator&lt;char&gt;::allocate(unsigned long)'
clang-14: error: linker command failed with exit code 1 (use -v to see invocation)
```

This manifests for `-std=c++20`, `-std=gnu++20`, `-std=c++2b`, `-std=gnu++2b`, but not for `std=c++17`. Additionally, the g++ can compile the code with the same flags, for every C++ standard I tested, from c++17 to c++2b.

See attached tarball for a minimal reproducible example and a script to test whether clang++ and g++ can build it for each standard.

I wasn't sure if this qualifies as release blocker because theoretically you could remove `-finstrument-functions`, in which case it compiles just fine. However I like seeing all the function names in debugging tools like ASAN.
</details>


---

