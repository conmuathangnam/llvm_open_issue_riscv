# Trivial copy assignment operator overwrites members not being copied

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/36386

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [37038](https://llvm.org/bz37038) |
| Version | trunk |
| OS | All |
| CC | @DougGregor |

## Extended Description 
In the following, the `operator=` used by Q's defaulted copy assignment operator for the `d` subobject is a copy assignment operator of `B`: that is, it does not copy the `b0` member of `d`.
The expected behaviour is observed when Q's copy assignment operator is explicitly defaulted after its first declaration, but not if it is explicitly defaulted on its first declaration.

### SOURCE (<stdin>):
```cpp
extern "C" void abort(void);
struct B0 { int b0; };

struct B {
  B &operator=(const B &) = default;
  int x;
};

struct D : B0, B {
  using B::operator=;
private:
  D &operator=(const D &) && = default;
};

struct Q {
  Q &operator=(const Q &) = default;
  D d;
};
//Q &Q::operator=(const Q &) = default;

Q qa, qb;
int main(void) {
  qb.d.b0 = 42;
  qb = qa;
  if (qb.d.b0 != 42) abort();
}
```

### COMPILER INVOCATION:
```
clang++ -xc++ -std=c++2a -o inhtrivcpyassn -
```

### INVOCATION AND OUTPUT OF RESULTING EXECUTABLE:
```
> ./inhtrivcpyassn; printf '(rc=%d)\n' "$?"
Aborted                 ./inhtrivcpyassn
(rc=134)
```

### EXPECTED RUN OUTPUT:
```
(rc=0)
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 7.0.0 (https://github.com/llvm-mirror/clang.git f38834898ad991aec557e2a609c6b7c3b288bf10) (https://github.com/llvm-mirror/llvm.git 433eb70569ee6379cdd5181e72b9193fdb0a39dd)
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

### Comment 1 - frederick-vs-ja

GCC, MSVC, and EDG behave the same.
- https://godbolt.org/z/hhxjjPMv8
- https://godbolt.org/z/PMocs4M86

Do we really want to allow trivial-but-skipping assignment operators? [P3279R0](https://wg21.link/p3279r0) seems related.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [37038](https://llvm.org/bz37038) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->DougGregor |

## Extended Description 
In the following, the `operator=` used by Q's defaulted copy assignment operator for the `d` subobject is a copy assignment operator of `B`: that is, it does not copy the `b0` member of `d`.
The expected behaviour is observed when Q's copy assignment operator is explicitly defaulted after its first declaration, but not if it is explicitly defaulted on its first declaration.

### SOURCE (&lt;stdin&gt;):
```cpp
extern "C" void abort(void);
struct B0 { int b0; };

struct B {
  B &amp;operator=(const B &amp;) = default;
  int x;
};

struct D : B0, B {
  using B::operator=;
private:
  D &amp;operator=(const D &amp;) &amp;&amp; = default;
};

struct Q {
  Q &amp;operator=(const Q &amp;) = default;
  D d;
};
//Q &amp;Q::operator=(const Q &amp;) = default;

Q qa, qb;
int main(void) {
  qb.d.b0 = 42;
  qb = qa;
  if (qb.d.b0 != 42) abort();
}
```

### COMPILER INVOCATION:
```
clang++ -xc++ -std=c++2a -o inhtrivcpyassn -
```

### INVOCATION AND OUTPUT OF RESULTING EXECUTABLE:
```
&gt; ./inhtrivcpyassn; printf '(rc=%d)\n' "$?"
Aborted                 ./inhtrivcpyassn
(rc=134)
```

### EXPECTED RUN OUTPUT:
```
(rc=0)
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 7.0.0 (https://github.com/llvm-mirror/clang.git f38834898ad991aec557e2a609c6b7c3b288bf10) (https://github.com/llvm-mirror/llvm.git 433eb70569ee6379cdd5181e72b9193fdb0a39dd)
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

