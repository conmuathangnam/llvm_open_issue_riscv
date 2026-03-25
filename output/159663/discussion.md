# [Clang] Lambda in a `decltype()` in template parameters causing incorrect type mismatches

**Author:** Metadorius
**URL:** https://github.com/llvm/llvm-project/issues/159663

## Body

The following code (minimal repro. example)
```cpp
template <typename T>
struct X {};

template <typename>
using DistinctCollector = X<decltype([]{})>;

DistinctCollector<int*> getCellSpreadItems()
{
	DistinctCollector<int*> set;
	return set;
}

int main() { getCellSpreadItems(); }
```

https://godbolt.org/z/obMWeKjKr

fails with

`error: no viable conversion from returned value of type 'DistinctCollector<...>' to function return type 'DistinctCollector<...>'`

The same error is not observed on GCC or MSVC (in conformance mode, iow `/permissive-`) compilers.

If you use

```cpp
auto getCellSpreadItems()
{
	DistinctCollector<int*> set;
	return set;
}
```
https://godbolt.org/z/4fnocEr13

it works.

Per @Sirraide on Discord:
> Looking at the AST, it seems like we’re instantiating that template twice; so it seems what it’s complaining about is that the two instances of `DistinctCollector<T>` are ... not the same type, which er, is not quite right
> There are some things we try to diagnose even if the function is still dependent, but usually you need to instantiate it yes, and this also looks like a problem specifically w/ instantiation

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Kerbiter (Metadorius)

<details>
The following code (minimal repro. example)
```cpp
template &lt;typename T&gt;
struct X {};

template &lt;typename&gt;
using DistinctCollector = X&lt;decltype([]{})&gt;;

DistinctCollector&lt;int*&gt; getCellSpreadItems()
{
	DistinctCollector&lt;int*&gt; set;
	return set;
}

int main() { getCellSpreadItems(); }
```

https://godbolt.org/z/obMWeKjKr

fails with

`error: no viable conversion from returned value of type 'DistinctCollector&lt;...&gt;' to function return type 'DistinctCollector&lt;...&gt;'`

The same error is not observed on GCC or MSVC (in conformance mode, iow `/permissive-`) compilers.

If you use

```cpp
auto getCellSpreadItems()
{
	DistinctCollector&lt;int*&gt; set;
	return set;
}
```
https://godbolt.org/z/4fnocEr13

it works.

Per @<!-- -->Sirraide on Discord:
&gt; Looking at the AST, it seems like we’re instantiating that template twice; so it seems what it’s complaining about is that the two instances of `DistinctCollector&lt;T&gt;` are ... not the same type, which er, is not quite right
&gt; There are some things we try to diagnose even if the function is still dependent, but usually you need to instantiate it yes, and this also looks like a problem specifically w/ instantiation
</details>


---

### Comment 2 - Sirraide

Looks like this has been an issue since Clang 13, which is the earliest release that supports lambdas in unevaluated operands.

---

### Comment 3 - cor3ntin

This is the dreaded [CWG2794](https://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2794)

---

