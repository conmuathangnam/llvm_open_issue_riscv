# Accepts-invalid (?): clang allows accessing private typedef of specialization in the specialization's template

**Author:** nico
**URL:** https://github.com/llvm/llvm-project/issues/22724

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [22350](https://llvm.org/bz22350) |
| Version | unspecified |
| OS | All |
| CC | @DougGregor |

## Extended Description 
clang accepts the following code, which gcc, cl, icc all reject. icc says `warning #525: type "A<void>::const_pointer" (declared at line 3) is an inaccessible type (allowed for cfront compatibility)`, so this is probably a clang bug (?)

```console
$ cat repro.ii
```
```cpp 
template <class T1> class A;
template <> class A<void> {
  typedef void *const_pointer;
};
template <class T1> class A {
  void m_fn1(A<void>::const_pointer);
};
class F {
  A<int> const_pointer;
};
```
```console
$ bin/clang -c repro.ii
$ gcc-4.8.1 -c repro.ii 
gcc-4.8.1: warning: couldn’t understand kern.osversion ‘14.0.0
repro.ii: In instantiation of ‘class A<int>’:
repro.ii:9:10:   required from here
repro.ii:3:17: error: ‘typedef void* A<void>::const_pointer’ is private
   typedef void *const_pointer;
                 ^
repro.ii:6:23: error: within this context
   void m_fn1(A<void>::const_pointer);
                       ^
```

## Comments

### Comment 1 - nico

*** Bug llvm/llvm-project#22701 has been marked as a duplicate of this bug. ***

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nico Weber (nico)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [22350](https://llvm.org/bz22350) |
| Version | unspecified |
| OS | All |
| CC | @<!-- -->DougGregor |

## Extended Description 
clang accepts the following code, which gcc, cl, icc all reject. icc says `warning #<!-- -->525: type "A&lt;void&gt;::const_pointer" (declared at line 3) is an inaccessible type (allowed for cfront compatibility)`, so this is probably a clang bug (?)

```console
$ cat repro.ii
```
```cpp 
template &lt;class T1&gt; class A;
template &lt;&gt; class A&lt;void&gt; {
  typedef void *const_pointer;
};
template &lt;class T1&gt; class A {
  void m_fn1(A&lt;void&gt;::const_pointer);
};
class F {
  A&lt;int&gt; const_pointer;
};
```
```console
$ bin/clang -c repro.ii
$ gcc-4.8.1 -c repro.ii 
gcc-4.8.1: warning: couldn’t understand kern.osversion ‘14.0.0
repro.ii: In instantiation of ‘class A&lt;int&gt;’:
repro.ii:9:10:   required from here
repro.ii:3:17: error: ‘typedef void* A&lt;void&gt;::const_pointer’ is private
   typedef void *const_pointer;
                 ^
repro.ii:6:23: error: within this context
   void m_fn1(A&lt;void&gt;::const_pointer);
                       ^
```
</details>


---

### Comment 3 - shafik

Looks like clang still gets this wrong: https://godbolt.org/z/4Eh743zP1

---

