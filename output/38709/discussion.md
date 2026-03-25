# Overload resolution depends on declaration order with `-fms-compatibility`

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/38709

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [39361](https://llvm.org/bz39361) |
| Version | trunk |
| OS | All |
| CC | @DougGregor |

## Extended Description 
Overload resolution with `-fms-compatibility` seems to be dependent on the order of declarations. The overload sets f and g are parallels of each other, but parallel calls do not resolve the same.

### SOURCE (<stdin>):
```cpp
char *f(float, int);
template <typename T> int f(long, T);
template <typename T> int g(long, T);
char *g(float, int);

bool zip(int x) { return f(x, x) == g(x, x); }
```

### COMPILER INVOCATION COMMAND:
```
clang++ -cc1 -triple i386-unknown-linux-gnu -fsyntax-only -x c++ -std=c++03 -fms-compatibility -
```

### ACTUAL COMPILER OUTPUT:
```
<stdin>:6:34: error: comparison between pointer and integer ('char *' and 'int')
bool zip(int x) { return f(x, x) == g(x, x); }
                         ~~~~~~~ ^  ~~~~~~~
1 error generated.
```

### EXPECTED COMPILER OUTPUT:
(Clean compile).


### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 8.0.0 (https://github.com/llvm-mirror/clang.git 2abe10f0902311fdb2896d3c6223787f51049fac) (https://github.com/llvm-mirror/llvm.git 4aeb7d033aef4e4345e52330160ea21ade905859)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5.4.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6.0.0
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5.4.0
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot

I think the reason is that the following has not considered condition fully. It needs return `ImplicitConversionSequence::Worse` so that the caller outside would not go to path to choose non-template candidate as first.
```cpp
  // In Microsoft mode, prefer an integral conversion to a
  // floating-to-integral conversion if the integral conversion
  // is between types of the same size.
  // For example:
  // void f(float);
  // void f(int);
  // int main {
  //    long a;
  //    f(a);
  // }
  // Here, MSVC will call f(int) instead of generating a compile error
  // as clang will do in standard mode.
  if (S.getLangOpts().MSVCCompat && SCS1.Second == ICK_Integral_Conversion &&
      SCS2.Second == ICK_Floating_Integral &&
      S.Context.getTypeSize(SCS1.getFromType()) ==
          S.Context.getTypeSize(SCS1.getToType(2)))
    return ImplicitConversionSequence::Better;
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [39361](https://llvm.org/bz39361) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->DougGregor |

## Extended Description 
Overload resolution with `-fms-compatibility` seems to be dependent on the order of declarations. The overload sets f and g are parallels of each other, but parallel calls do not resolve the same.

### SOURCE (&lt;stdin&gt;):
```cpp
char *f(float, int);
template &lt;typename T&gt; int f(long, T);
template &lt;typename T&gt; int g(long, T);
char *g(float, int);

bool zip(int x) { return f(x, x) == g(x, x); }
```

### COMPILER INVOCATION COMMAND:
```
clang++ -cc1 -triple i386-unknown-linux-gnu -fsyntax-only -x c++ -std=c++03 -fms-compatibility -
```

### ACTUAL COMPILER OUTPUT:
```
&lt;stdin&gt;:6:34: error: comparison between pointer and integer ('char *' and 'int')
bool zip(int x) { return f(x, x) == g(x, x); }
                         ~~~~~~~ ^  ~~~~~~~
1 error generated.
```

### EXPECTED COMPILER OUTPUT:
(Clean compile).


### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 8.0.0 (https://github.com/llvm-mirror/clang.git 2abe10f0902311fdb2896d3c6223787f51049fac) (https://github.com/llvm-mirror/llvm.git 4aeb7d033aef4e4345e52330160ea21ade905859)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5.4.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6.0.0
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5.4.0
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 3 - shafik

We don't seem to get a diagnosic anymore, @hubert-reinterpretcast does this look good to you now?

---

### Comment 4 - hubert-reinterpretcast

> We don't seem to get a diagnosic anymore, [@hubert-reinterpretcast](https://github.com/hubert-reinterpretcast) does this look good to you now?

Still reproduces: https://godbolt.org/z/KffMjavev

Flags:
-std=c++2c
--target=x86_64-pc-windows-msvc
-fms-compatibility
-fms-compatibility-version=19.27

---

### Comment 5 - shafik

CC @AaronBallman 

---

