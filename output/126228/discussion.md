# Clang still fails to compile correct program with consteval in LLVM 19.1.7

**Author:** 3manifold
**URL:** https://github.com/llvm/llvm-project/issues/126228
**Status:** Closed
**Labels:** duplicate, clang:frontend, consteval
**Closed Date:** 2025-02-07T11:29:09Z

## Body

Is it supposed that  issue https://github.com/llvm/llvm-project/issues/105558 was fixed in `LLVM 19.1.7`? I tried to compile the example in the aforementioned link under `Ubuntu 25.04 (Plucky Puffin)` and still fails. 

## clang
```
> clang++ --version
Ubuntu clang version 19.1.7 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
```
```
> clang++ -Wall -std=c++23  playground_main.cpp   -o test && ./test
playground_main.cpp:24:40: error: call to consteval function 'foo' is not a constant expression
   24 |     cout << (make_static<foo().size()>(foo()) | ranges::to<string>());
      |                                        ^
playground_main.cpp:24:40: note: pointer to subobject of heap-allocated object is not a constant expression
/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/allocator.h:193:31: note: heap allocation performed here
  193 |             return static_cast<_Tp*>(::operator new(__n));
      |                                      ^
1 error generated.
```

## gcc
```
> g++ --version
g++ (Ubuntu 14.2.0-16ubuntu1) 14.2.0
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
```
> g++ -Wall -std=c++23  playground_main.cpp   -o test && ./test
some dynamic data 42
```

## System specs
```
> cat /etc/lsb-release
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=25.04
DISTRIB_CODENAME=plucky
DISTRIB_DESCRIPTION="Ubuntu Plucky Puffin (development branch)"
```


## Comments

### Comment 1 - frederick-vs-ja

The fix only went to Clang 20 and wasn't backported to Clang 19. See also c821cc3f8824e29ba1861fb1de43e90543928498.

Closing as duplicate of the mentioned issue.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (3manifold)

<details>
Is it supposed that  issue https://github.com/llvm/llvm-project/issues/105558 was fixed in `LLVM 19.1.7`? I tried to compile the example in the aforementioned link under `Ubuntu 25.04 (Plucky Puffin)` and still fails. 

## clang
```
&gt; clang++ --version
Ubuntu clang version 19.1.7 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
```
```
&gt; clang++ -Wall -std=c++23  playground_main.cpp   -o test &amp;&amp; ./test
playground_main.cpp:24:40: error: call to consteval function 'foo' is not a constant expression
   24 |     cout &lt;&lt; (make_static&lt;foo().size()&gt;(foo()) | ranges::to&lt;string&gt;());
      |                                        ^
playground_main.cpp:24:40: note: pointer to subobject of heap-allocated object is not a constant expression
/usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/allocator.h:193:31: note: heap allocation performed here
  193 |             return static_cast&lt;_Tp*&gt;(::operator new(__n));
      |                                      ^
1 error generated.
```

## gcc
```
&gt; g++ --version
g++ (Ubuntu 14.2.0-16ubuntu1) 14.2.0
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
```
&gt; g++ -Wall -std=c++23  playground_main.cpp   -o test &amp;&amp; ./test
some dynamic data 42
```

## System specs
```
&gt; cat /etc/lsb-release
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=25.04
DISTRIB_CODENAME=plucky
DISTRIB_DESCRIPTION="Ubuntu Plucky Puffin (development branch)"
```

</details>


---

