# Conditional expression accepted despite ambiguous common type: derived-to-base and base-to-derived UDC

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/23380

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [23006](https://llvm.org/bz23006) |
| Version | trunk |
| OS | All |
| CC | @DougGregor,@zygoloid |

## Extended Description 
Conditional expressions where the second and third operands have different types are ill-formed if conversion can be done both ways.

Clang fails to reject the conditional expression in the case below where one conversion is derived-to-base with slicing and the other is a user-defined conversion.

In the case below, this manifests as an error because the wrong partial specialization is chosen.

### SOURCE (`<stdin>`):
```cpp
struct A;

struct B {
   operator A &&();
} bObj;

struct A : B { };

enum E { Gnat };
A &&fooA(E);

bool b;

template <typename T, typename U = char [sizeof 0]> struct Hack { };
template <typename T> struct Hack<T, char [sizeof ((b ? bObj : fooA(T())), 0)]>;

void bar() {
   A &&t1 = bObj;
   B t2 = fooA(Gnat);
   Hack<E> hack;
}
```

### COMPILER INVOCATION:
```
clang++ -cc1 -std=c++11 -x c++ -
```

### ACTUAL OUTPUT:
```
<stdin>:20:12: error: implicit instantiation of undefined template 'Hack<E, char [4]>'
   Hack<E> hack;
           ^
<stdin>:15:30: note: template is declared here
template <typename T> struct Hack<T, char [sizeof ((b ? bObj : fooA(T())), 0)]>;
                             ^
1 error generated.
```

### EXPECTED OUTPUT:
(Successful compile).


### COMPILER VERSION INFO:
```
clang version 3.7.0 (trunk 232951)
Target: x86_64-unknown-linux-gnu
Thread model: posix
Found candidate GCC installation: /usr/local/gcc-4.8.2/lib/gcc/x86_64-unknown-linux-gnu/4.8.2
Selected GCC installation: /usr/local/gcc-4.8.2/lib/gcc/x86_64-unknown-linux-gnu/4.8.2
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Reduced testcase:

```cpp
  struct B { operator struct A &&(); } bObj;
  struct A : B {};
  auto x = true ? bObj : static_cast<A&&>(A());
```

I think Clang interprets the "if neither of the conversions above can be done" in 5.16/3.3 as meaning "if neither of the conversions above can be done *in either direction*".

---

### Comment 2 - hubert-reinterpretcast

### ANALYSIS:
A walkthrough of N4296 subclause 5.16 [expr.cond] paragraph 3.

E1: lvalue of type B
E2: xvalue of type A

E2 (xvalue of type A) is not an lvalue.
E2 (xvalue of type A) is an xvalue:
- okay, E1 can be converted to match E2: "T2 &&" is "A &&".

the implicit conversion can be done (and binds directly to the result of the conversion function):
A &&t1 = bObj; // okay; we get to 8.5.3/5 (5.2.1.2) and A is not reference-related to B


E1: xvalue of type A
E2: lvalue of type B

E2 (lvalue of type B) is an lvalue:
- E1 (xvalue of type A) cannot be implicitly converted to B &.
E2 (lvalue of type B) is not an xvalue.
Neither of the conversions above can be done:
- E1 and E2 have class type; the underlying class types are the same or one is a base class of the other.
  The class of T2 (B) is a base class of the class of T1 (A); the cv-qualification of T2 is the same as that of T1.
  E1 can be converted to match E2.

B t2 = fooA(Gnat); // okay; slices via the move constructor of B

Both can be converted. The conditional expression is ill-formed.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [23006](https://llvm.org/bz23006) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
Conditional expressions where the second and third operands have different types are ill-formed if conversion can be done both ways.

Clang fails to reject the conditional expression in the case below where one conversion is derived-to-base with slicing and the other is a user-defined conversion.

In the case below, this manifests as an error because the wrong partial specialization is chosen.

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A;

struct B {
   operator A &amp;&amp;();
} bObj;

struct A : B { };

enum E { Gnat };
A &amp;&amp;fooA(E);

bool b;

template &lt;typename T, typename U = char [sizeof 0]&gt; struct Hack { };
template &lt;typename T&gt; struct Hack&lt;T, char [sizeof ((b ? bObj : fooA(T())), 0)]&gt;;

void bar() {
   A &amp;&amp;t1 = bObj;
   B t2 = fooA(Gnat);
   Hack&lt;E&gt; hack;
}
```

### COMPILER INVOCATION:
```
clang++ -cc1 -std=c++11 -x c++ -
```

### ACTUAL OUTPUT:
```
&lt;stdin&gt;:20:12: error: implicit instantiation of undefined template 'Hack&lt;E, char [4]&gt;'
   Hack&lt;E&gt; hack;
           ^
&lt;stdin&gt;:15:30: note: template is declared here
template &lt;typename T&gt; struct Hack&lt;T, char [sizeof ((b ? bObj : fooA(T())), 0)]&gt;;
                             ^
1 error generated.
```

### EXPECTED OUTPUT:
(Successful compile).


### COMPILER VERSION INFO:
```
clang version 3.7.0 (trunk 232951)
Target: x86_64-unknown-linux-gnu
Thread model: posix
Found candidate GCC installation: /usr/local/gcc-4.8.2/lib/gcc/x86_64-unknown-linux-gnu/4.8.2
Selected GCC installation: /usr/local/gcc-4.8.2/lib/gcc/x86_64-unknown-linux-gnu/4.8.2
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 4 - shafik

clang and gcc agree on the original example, the reduced example they also agree but not edg/MSVC and the result is interesting: https://godbolt.org/z/75nWj5bof

@hubert-reinterpretcast I think clang and gcc result make sense, wdyt?

---

