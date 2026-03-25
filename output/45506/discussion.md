# clang/libclang crashes on parsing (std >= c++14) on all versions (recursion bug)

**Author:** 0a0075c6-366a-4feb-99d7-c23f73cb7fe9
**URL:** https://github.com/llvm/llvm-project/issues/45506
**Status:** Closed
**Labels:** c++14, clang:frontend, bugzilla, crash
**Closed Date:** 2025-03-10T16:31:33Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46161](https://llvm.org/bz46161) |
| Version | unspecified |
| OS | Linux |
| Attachments | [Code source that causes the crash.](https://user-images.githubusercontent.com/95090627/143760814-c2ab8a59-f21e-49b5-b632-5ebc5e0253e5.gz) |
| CC | @zygoloid |

## Extended Description 


Hello dear LLVM/clang developers.

I've discovered a crash with clang that seem to happen on every versions when std >= 14.
I have tested with clang version 8,9,10,11, it has the same problem everywhere.

My thoughts are that it seems to happen when one template function have the same name, same arguments and template arguments, when using template parameters packs as last template argument.

When clang tries to parse it, it is processing the template `function1` in compile-time for creating the `function1` that will be used next for compiling, and clang resolves it recursively by calling the wrong template `function1` instead of calling template `function2`.

template `function1` and template `function2` have the same function name (to avoid confusion) of course.

Results:
```
1.      main.cpp:24:29: current parser token ')'
2.      main.cpp:23:1: parsing function body 'main'
3.      main.cpp:23:1: in compound statement ('{}')
4.      ./types.h:73:27: instantiating function definition 'XLib::CVFunc<0, void (*)()>'
5.      ./types.h:61:27: instantiating function definition 'XLib::VFunc<0, void (*)()>'
6.      ./types.h:61:27: instantiating function definition 'XLib::VFunc<0, void (*(*)())()>'
7.      ./types.h:61:27: instantiating function definition 'XLib::VFunc<0, void (*(*(*)())())()>'
8.      ./types.h:61:27: instantiating function definition 'XLib::VFunc<0, void (*(*(*(*)())())())()>'
```

I've attached an example of source code inside the attachments so you can debug it yourself because it is easily reproducible, in hope it could help.

Commands used:
```
clang-8 -std=c++14 -o main main.cpp  
clang-9 -std=c++14 -o main main.cpp  
clang-11 -std=c++14 -o main main.cpp  
clang-11 -std=c++14 -o main main.cpp
```

I would be glad that it could be fixed, even if it's my coding style that isn't very pleasant...

Thank you!

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

These two commits:

https://github.com/llvm/llvm-project/commit/26a92d5852b2c6bf77efd26f6c0194c913f40285
https://github.com/llvm/llvm-project/commit/471d06020a6a12c621131c508e58878890db3906

might be enough.

---

### Comment 2 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

I would really appreciate that if you can so I can build it myself,
thank you.

---

### Comment 3 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Sorry, we are no longer providing patch releases for Clang 8. However, I could dig out the patch that introduced the fix if you'd like, it might possibly apply cleanly to the Clang 8 sources.

---

### Comment 4 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

Yes it is using libclang8.
Alright I see thank you,
do you think it would be possible to make this little update for libclang8? 
Also this is weird because I tried with lastest libclang (11) and it still crashed but maybe its just my bad memory. Ill try this again properly.
Thanks!

---

### Comment 5 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

OK, so the crash happens inside:
```
#10371 0x00007fff0d89b6af in clang_parseTranslationUnit_Impl () at /build/llvm-toolchain-8-bJQSSk/llvm-toolchain-8-8/tools/clang/tools/libclang/CIndex.cpp:3487
#10372 operator() () at /build/llvm-toolchain-8-bJQSSk/llvm-toolchain-8-8/tools/clang/tools/libclang/CIndex.cpp:3570
```
in a thread created by
```
#4  clang_parseTranslationUnit2FullArgv () at /build/llvm-toolchain-8-bJQSSk/llvm-toolchain-8-8/tools/clang/tools/libclang/CIndex.cpp:3577
#5  0x00007fff0d886d8f in clang_parseTranslationUnit2 () at /build/llvm-toolchain-8-bJQSSk/llvm-toolchain-8-8/tools/clang/tools/libclang/CIndex.cpp:3549
```
... which does call `noteBottomOfStack()` in modern versions of Clang. So it looks like this particular crash was fixed by

https://github.com/llvm/llvm-project/commit/471d06020a6a12c621131c508e58878890db3906

... for Clang 10. (I would guess, based on the paths here, that KDevelop is using libclang from Clang 8.)

---

### Comment 6 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

[Another backtrace with llvm symbols](https://user-images.githubusercontent.com/95090627/143760815-395dd478-5b7d-4ff9-995d-8880dc041fe2.gz)
Sorry I just forgot the libllvm symbols, I hope it should help better.

---

### Comment 7 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

In case you want to test it yourself, you have just to set c++14 in the project configuration of kdevelop to make it parse.

Project -> Open configuration -> Language settings -> C/C++ Parser -> C++ Profile c++14

---

### Comment 8 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

[backtrace and kdevelop project that causes the libclang crash](https://user-images.githubusercontent.com/95090627/143760816-1c3f2d30-68b7-4bef-9dac-11ec99c33bf1.gz)
I've added the backtrace to the attachments, in hope it can help you.

---

### Comment 9 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Clang detects problems if it gets "near" the end of the stack (within 256KiB of stack exhaustion), so it should be OK if we actually get a little less than the full 8MiB. (We are also OK with a little stack space being used before noteBottomOfStack is called.)

---

### Comment 10 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

It could be that `clang::noteBottomOfStack()` was never called in the thread where the crash occurs. Do you have a complete backtrace from such a crash that you could share?

---

### Comment 11 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

Actually, it does allocate just enough memory but it doesn't use the whole stack (I don't really know why yet I need to look on pthread)

---

### Comment 12 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

Thank you for the informations.
After what you said I've looked a bit how the kde application worked (kdevelop).
First I didn't see any references to DesiredStackSize which makes senses because it's the pthread library that calls the clone syscall (https://www.man7.org/linux/man-pages/man2/clone.2.html) in the end.
It sets the stack and the stack size as parameters before calling the real syscall clone. (or clone_args structure for newer versions of kernels)

I just checked a bit on how it does work in my system.

The rsi register on glibc before calling the real clone syscall is for example 0x00007fff8e0a3b00 and is where the stack starts, if you check the memory maps it gives

```
7fff8d8a5000-7fff8e0a5000 rw-p 00000000 00:00 0 
7fff8e0a5000-7fff8e0a6000 ---p 00000000 00:00 0 
7fff8e0a6000-7fff8e8a6000 rw-p 00000000 00:00 0 
```

And this is where it gets interesting,
```
0x00007fff8e0a3b00 - 0x7fff8d8a5000
= 8383232

8<<20
= 8388608
```
The `DesiredStackSize` on clang is greater than what pthread allocates for the stack, even if it's not really a lot, it is enough to crash the library if I understand what you said.

I don't know if it should be fixed either in pthread to make a new function with a new stack size parameter or in clang by lowering a bit the value, but I think it should be fixable easily.

---

### Comment 13 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Users of clang as a library are required invoke clang on a thread with a stack size of at least `clang::DesiredStackSize`, and are required to call `clang::noteBottomOfStack()` somewhere near the bottom of that stack (https://github.com/llvm/llvm-project/blob/master/clang/include/clang/Basic/Stack.h#L30).

libclang should generally take care of that for you, unless you set the environment variable LIBCLANG_NOTHREADS (https://github.com/llvm/llvm-project/blob/31f40f603d0c00b313397196124c5f39090badf0/clang/tools/libclang/CIndex.cpp#L3691). If that's not working, that seems like a bug.

---

### Comment 14 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

Oh I see but it seems that the option isn't enabled by default right?
Because most of applications that uses libclang to parse >= c++14 code crashes (kdevelop for example)
I reported the bug here because they (KDE authors) told me it was a bug with clang, but apparently not.
Thanks.

---

### Comment 15 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Clang now produces this diagnostic:
```
<source>:67:16: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
```
... and then:
```
<source>:67:16: fatal error: recursive template instantiation exceeded maximum depth of 1024
```
... and does not crash: https://godbolt.org/z/NLNRBs

So this is not a crasher bug any more. I've not yet confirmed whether what Clang is doing (the recursive instantiation) is correct, but I suspect it is, and the testcase is invalid.

---

### Comment 16 - 0a0075c6-366a-4feb-99d7-c23f73cb7fe9

Just adding some additional information,

g++ seems to have the same issue, except it avoid the crashes by setting a maximum template instantiation depth with -ftemplate-depth option (900 by default)

I don't really know what would be the best approach to fix with c++ current standards, even though it is maybe my code who is simply wrong and even illegal. 

Though if template functions can be created this way since variadic template can have no arguments, maybe doing something like a keyword (like explicit) so that the compiler can use so it knows what template function to use (instead of recursively calling the wrong one and ends up to crash the compiler).


Thank you for your replies in advance!

---

### Comment 17 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (0a0075c6-366a-4feb-99d7-c23f73cb7fe9)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [46161](https://llvm.org/bz46161) |
| Version | unspecified |
| OS | Linux |
| Attachments | [Code source that causes the crash.](https://user-images.githubusercontent.com/95090627/143760814-c2ab8a59-f21e-49b5-b632-5ebc5e0253e5.gz) |
| CC | @<!-- -->zygoloid |

## Extended Description 


Hello dear LLVM/clang developers.

I've discovered a crash with clang that seem to happen on every versions when std &gt;= 14.
I have tested with clang version 8,9,10,11, it has the same problem everywhere.

My thoughts are that it seems to happen when one template function have the same name, same arguments and template arguments, when using template parameters packs as last template argument.

When clang tries to parse it, it is processing the template `function1` in compile-time for creating the `function1` that will be used next for compiling, and clang resolves it recursively by calling the wrong template `function1` instead of calling template `function2`.

template `function1` and template `function2` have the same function name (to avoid confusion) of course.

Results:
```
1.      main.cpp:24:29: current parser token ')'
2.      main.cpp:23:1: parsing function body 'main'
3.      main.cpp:23:1: in compound statement ('{}')
4.      ./types.h:73:27: instantiating function definition 'XLib::CVFunc&lt;0, void (*)()&gt;'
5.      ./types.h:61:27: instantiating function definition 'XLib::VFunc&lt;0, void (*)()&gt;'
6.      ./types.h:61:27: instantiating function definition 'XLib::VFunc&lt;0, void (*(*)())()&gt;'
7.      ./types.h:61:27: instantiating function definition 'XLib::VFunc&lt;0, void (*(*(*)())())()&gt;'
8.      ./types.h:61:27: instantiating function definition 'XLib::VFunc&lt;0, void (*(*(*(*)())())())()&gt;'
```

I've attached an example of source code inside the attachments so you can debug it yourself because it is easily reproducible, in hope it could help.

Commands used:
```
clang-8 -std=c++14 -o main main.cpp  
clang-9 -std=c++14 -o main main.cpp  
clang-11 -std=c++14 -o main main.cpp  
clang-11 -std=c++14 -o main main.cpp
```

I would be glad that it could be fixed, even if it's my coding style that isn't very pleasant...

Thank you!
</details>


---

