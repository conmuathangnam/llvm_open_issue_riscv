# [clang] Ambiguity between function declaration and definition when using pack indexing

**Author:** FSK-idk
**URL:** https://github.com/llvm/llvm-project/issues/123033
**Status:** Closed
**Labels:** clang:frontend, confirmed
**Closed Date:** 2025-01-16T16:50:32Z

## Body

Godbolt: https://godbolt.org/z/817Kve9zc

It seems that clang can't compile code if I decide to separate the declaration and implementation of a function with pack indexing in some cases. However, if you comment out the declaration, it will compile.

```cpp
#include <concepts>
#include <iostream>


// in requires expression

template <class... Types>
requires std::same_as<Types...[0], int>
void print(double d);

template <class... Types>
requires std::same_as<Types...[0], int>
void print(double d) {
    std::cout << "it doesn't work " << static_cast<Types...[0]>(d) << "\n";
}


// in return type

template <class... Types>
Types...[0] convert(double d);

template <class... Types>
Types...[0] convert(double d) {
    return static_cast<Types...[0]>(d);
}


int main() {
    print<int, int>(12.34);
    std::cout << "convert: " << convert<int, int>(12.34) << "\n";

    return 0;
}
```

Output
```
<source>:30:5: error: call to 'print' is ambiguous
   30 |     print<int, int>(12.34);
      |     ^~~~~~~~~~~~~~~
<source>:9:6: note: candidate function [with Types = <int, int>]
    9 | void print(double d);
      |      ^
<source>:13:6: note: candidate function [with Types = <int, int>]
   13 | void print(double d) {
      |      ^
<source>:31:33: error: call to 'convert' is ambiguous
   31 |     std::cout << "convert: " << convert<int, int>(12.34) << "\n";
      |                                 ^~~~~~~~~~~~~~~~~
<source>:21:13: note: candidate function [with Types = <int, int>]
   21 | Types...[0] convert(double d);
      |             ^
<source>:24:13: note: candidate function [with Types = <int, int>]
   24 | Types...[0] convert(double d) {
      |             ^
2 errors generated.
Compiler returned: 1
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: FSK (FSK-idk)

<details>
Godbolt: https://godbolt.org/z/817Kve9zc

It seems that clang can't compile code if I decide to separate the declaration and implementation of a function with pack indexing in some cases. However, if you comment out the declaration, it will compile.

```cpp
#include &lt;concepts&gt;
#include &lt;iostream&gt;


// in requires expression

template &lt;class... Types&gt;
requires std::same_as&lt;Types...[0], int&gt;
void print(double d);

template &lt;class... Types&gt;
requires std::same_as&lt;Types...[0], int&gt;
void print(double d) {
    std::cout &lt;&lt; "it doesn't work " &lt;&lt; static_cast&lt;Types...[0]&gt;(d) &lt;&lt; "\n";
}


// in return type

template &lt;class... Types&gt;
Types...[0] convert(double d);

template &lt;class... Types&gt;
Types...[0] convert(double d) {
    return static_cast&lt;Types...[0]&gt;(d);
}


int main() {
    print&lt;int, int&gt;(12.34);
    std::cout &lt;&lt; "convert: " &lt;&lt; convert&lt;int, int&gt;(12.34) &lt;&lt; "\n";

    return 0;
}
```

Output
```
&lt;source&gt;:30:5: error: call to 'print' is ambiguous
   30 |     print&lt;int, int&gt;(12.34);
      |     ^~~~~~~~~~~~~~~
&lt;source&gt;:9:6: note: candidate function [with Types = &lt;int, int&gt;]
    9 | void print(double d);
      |      ^
&lt;source&gt;:13:6: note: candidate function [with Types = &lt;int, int&gt;]
   13 | void print(double d) {
      |      ^
&lt;source&gt;:31:33: error: call to 'convert' is ambiguous
   31 |     std::cout &lt;&lt; "convert: " &lt;&lt; convert&lt;int, int&gt;(12.34) &lt;&lt; "\n";
      |                                 ^~~~~~~~~~~~~~~~~
&lt;source&gt;:21:13: note: candidate function [with Types = &lt;int, int&gt;]
   21 | Types...[0] convert(double d);
      |             ^
&lt;source&gt;:24:13: note: candidate function [with Types = &lt;int, int&gt;]
   24 | Types...[0] convert(double d) {
      |             ^
2 errors generated.
Compiler returned: 1
```


</details>


---

