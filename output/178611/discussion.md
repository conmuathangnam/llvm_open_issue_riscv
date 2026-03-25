# clang reject valid code: <> cmp operator confuse with template <>

**Author:** rockeet
**URL:** https://github.com/llvm/llvm-project/issues/178611
**Status:** Closed
**Labels:** duplicate, clang:frontend, rejects-valid
**Closed Date:** 2026-01-29T08:49:06Z

## Body

The reduced reproduction code:
```c++
#define TOPLING_PP_CAT2_1(a,b)    a##b
#define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
#define TOPLING_PP_ARG_X(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, \
           a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z, \
           A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,XX,...) XX
#define TOPLING_PP_ARG_N(...) \
        TOPLING_PP_ARG_X("ignored", ##__VA_ARGS__, \
            Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A, \
            z,y,x,w,v,u,t,s,r,q,p,o,n,m,l,k,j,i,h,g,f,e,d,c,b,a, \
                                            9,8,7,6,5,4,3,2,1,0)
#define TOPLING_PP_VA_NAME(prefix,...) \
        TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))

#define TOPLING_CMP1(d,f,o) \
    if (__x_ d f o __y_ d f) return true; \
    if (__y_ d f o __x_ d f) return false;

#define TOPLING_CMP_O_2(f,o)     return __x_.f o __y_.f;
#define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)

#define TOPLING_CMP(...) \
 [](const auto& __x_, const auto& __y_) ->bool { \
  TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }

#include <algorithm>
template<class T>
struct x {}; // name same with A::x
struct A { int x, y; };
void f(A* beg, A* end) {
    std::sort(beg, end, TOPLING_CMP(x, <, y, >));
}
```

gcc is ok, see: https://godbolt.org/z/vzG5ccKEY

clang compile error:
```
<source>:30:25: error: expected '>'
   30 |     std::sort(beg, end, TOPLING_CMP(x, <, y, >));
      |                         ^
<source>:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
<source>:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
<source>:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
<scratch space>:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
<source>:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
<source>:15:28: note: expanded from macro 'TOPLING_CMP1'
   15 |     if (__x_ d f o __y_ d f) return true; \
      |                            ^
<source>:30:40: note: to match this '<'
   30 |     std::sort(beg, end, TOPLING_CMP(x, <, y, >));
      |                                        ^
<source>:30:25: error: expected unqualified-id
   30 |     std::sort(beg, end, TOPLING_CMP(x, <, y, >));
      |                         ^
<source>:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
<source>:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
<source>:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
<scratch space>:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
<source>:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
<source>:15:28: note: expanded from macro 'TOPLING_CMP1'
   15 |     if (__x_ d f o __y_ d f) return true; \
      |                            ^
<source>:30:25: error: expected '>'
<source>:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
<source>:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
<source>:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
<scratch space>:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
<source>:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
<source>:16:28: note: expanded from macro 'TOPLING_CMP1'
   16 |     if (__y_ d f o __x_ d f) return false;
      |                            ^
<source>:30:40: note: to match this '<'
   30 |     std::sort(beg, end, TOPLING_CMP(x, <, y, >));
      |                                        ^
<source>:30:25: error: expected unqualified-id
   30 |     std::sort(beg, end, TOPLING_CMP(x, <, y, >));
      |                         ^
<source>:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
<source>:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
<source>:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
<scratch space>:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
<source>:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
<source>:16:28: note: expanded from macro 'TOPLING_CMP1'
   16 |     if (__y_ d f o __x_ d f) return false;
      |                            ^
4 errors generated.
```

## Comments

### Comment 1 - keinflue

Without irrelevant `std::sort` call and preprocessed:
```cpp
template<class T>
struct x {};
struct A { int x, y; };
void f(A* beg, A* end) {
    [](const auto& __x_, const auto& __y_) ->bool { if (__x_ . x < __y_ . x) return true; if (__y_ . x < __x_ . x) return false;return __x_.y > __y_.y; };
}
```
Further reduces to (https://godbolt.org/z/jcqEEdxrT)
```cpp
template<class T> struct x {};
auto lambda = [](const auto& a) { a.x < 0; };
```

Duplicate of https://github.com/llvm/llvm-project/issues/81731.

Please also note that identifiers starting with double underscore (`__x_` and `__y_`) are reserved to the implementation and shouldn't be used by user code.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: rockeet (rockeet)

<details>
The reduced reproduction code:
```c++
#define TOPLING_PP_CAT2_1(a,b)    a##b
#define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
#define TOPLING_PP_ARG_X(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, \
           a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z, \
           A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,XX,...) XX
#define TOPLING_PP_ARG_N(...) \
        TOPLING_PP_ARG_X("ignored", ##__VA_ARGS__, \
            Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A, \
            z,y,x,w,v,u,t,s,r,q,p,o,n,m,l,k,j,i,h,g,f,e,d,c,b,a, \
                                            9,8,7,6,5,4,3,2,1,0)
#define TOPLING_PP_VA_NAME(prefix,...) \
        TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))

#define TOPLING_CMP1(d,f,o) \
    if (__x_ d f o __y_ d f) return true; \
    if (__y_ d f o __x_ d f) return false;

#define TOPLING_CMP_O_2(f,o)     return __x_.f o __y_.f;
#define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)

#define TOPLING_CMP(...) \
 [](const auto&amp; __x_, const auto&amp; __y_) -&gt;bool { \
  TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }

#include &lt;algorithm&gt;
template&lt;class T&gt;
struct x {}; // name same with A::x
struct A { int x, y; };
void f(A* beg, A* end) {
    std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
}
```

gcc is ok, see: https://godbolt.org/z/vzG5ccKEY

clang compile error:
```
&lt;source&gt;:30:25: error: expected '&gt;'
   30 |     std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
      |                         ^
&lt;source&gt;:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
&lt;source&gt;:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
&lt;source&gt;:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
&lt;scratch space&gt;:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
&lt;source&gt;:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
&lt;source&gt;:15:28: note: expanded from macro 'TOPLING_CMP1'
   15 |     if (__x_ d f o __y_ d f) return true; \
      |                            ^
&lt;source&gt;:30:40: note: to match this '&lt;'
   30 |     std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
      |                                        ^
&lt;source&gt;:30:25: error: expected unqualified-id
   30 |     std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
      |                         ^
&lt;source&gt;:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
&lt;source&gt;:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
&lt;source&gt;:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
&lt;scratch space&gt;:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
&lt;source&gt;:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
&lt;source&gt;:15:28: note: expanded from macro 'TOPLING_CMP1'
   15 |     if (__x_ d f o __y_ d f) return true; \
      |                            ^
&lt;source&gt;:30:25: error: expected '&gt;'
&lt;source&gt;:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
&lt;source&gt;:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
&lt;source&gt;:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
&lt;scratch space&gt;:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
&lt;source&gt;:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
&lt;source&gt;:16:28: note: expanded from macro 'TOPLING_CMP1'
   16 |     if (__y_ d f o __x_ d f) return false;
      |                            ^
&lt;source&gt;:30:40: note: to match this '&lt;'
   30 |     std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
      |                                        ^
&lt;source&gt;:30:25: error: expected unqualified-id
   30 |     std::sort(beg, end, TOPLING_CMP(x, &lt;, y, &gt;));
      |                         ^
&lt;source&gt;:23:3: note: expanded from macro 'TOPLING_CMP'
   23 |   TOPLING_PP_VA_NAME(TOPLING_CMP_O_,__VA_ARGS__)(__VA_ARGS__) }
      |   ^
&lt;source&gt;:12:9: note: expanded from macro 'TOPLING_PP_VA_NAME'
   12 |         TOPLING_PP_CAT2(prefix,TOPLING_PP_ARG_N(__VA_ARGS__))
      |         ^
&lt;source&gt;:2:35: note: expanded from macro 'TOPLING_PP_CAT2'
    2 | #define TOPLING_PP_CAT2(a,b)      TOPLING_PP_CAT2_1(a,b)
      |                                   ^
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
&lt;scratch space&gt;:313:1: note: expanded from here
  313 | TOPLING_CMP_O_4
      | ^
&lt;source&gt;:19:34: note: expanded from macro 'TOPLING_CMP_O_4'
   19 | #define TOPLING_CMP_O_4(f,o,...) TOPLING_CMP1(. ,f,o)TOPLING_CMP_O_2(__VA_ARGS__)
      |                                  ^
&lt;source&gt;:16:28: note: expanded from macro 'TOPLING_CMP1'
   16 |     if (__y_ d f o __x_ d f) return false;
      |                            ^
4 errors generated.
```
</details>


---

