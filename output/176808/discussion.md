# 10_literal.call() doesn't work - User defined literals can't call methods

**Author:** Peach1
**URL:** https://github.com/llvm/llvm-project/issues/176808
**Status:** Closed
**Labels:** duplicate, clang:frontend
**Closed Date:** 2026-01-19T22:08:09Z

## Body

```cpp
"10"_literal.call(); // call works fine
  10_literal.call(); // error
```

10_literal.call(); does NOT work in clang - other C++ compilers allows this, clang does not.
"10"_literal.call(); works - but only for strings, not for integers.

How can clang be fixed so user-defined-literals can call methods on all literal types?

```cpp
struct A
{
    int call()
    {
        return 123;
    }
};

A operator ""_literal(unsigned long long x)
{
    return {};
}
A operator ""_literal(const char *str, decltype(sizeof(0)) len)
{
    return {};
}

void userdefined()
{
    "10"_literal.call(); // call works fine
    10_literal.call(); // error
    // ^ No matching literal operator for call to 'operator""_literal.call' with argument of type 'unsigned long long' or 'const char *', and no matching literal operator template
    // (it's parsed as '_a.call'  instead of '_a' - it should stop at the '.' like the StringLiteralParser does
    // is there an error in clang's NumericLiteralParse in clang/lib/Lex/LiteralSupport.cpp   ?
    
    (10_literal).call(); // call works fine
}


```
https://godbolt.org/z/vWhfzYbcc

It's parsed as '_literal.call'  instead of '_literal' - it should stop at the '.' like the StringLiteralParser does.
Is the error in clang's `NumericLiteralParser` in clang/lib/Lex/LiteralSupport.cpp   ?


## Comments

### Comment 1 - keinflue

This is a duplicate of https://github.com/llvm/llvm-project/issues/71535. I will continue discussing the issue there.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Peach1)

<details>
```cpp
"10"_literal.call(); // call works fine
  10_literal.call(); // error
```

10_literal.call(); does NOT work in clang - other C++ compilers allows this, clang does not.
"10"_literal.call(); works - but only for strings, not for integers.

How can clang be fixed so user-defined-literals can call methods on all literal types?

```cpp
struct A
{
    int call()
    {
        return 123;
    }
};

A operator ""_literal(unsigned long long x)
{
    return {};
}
A operator ""_literal(const char *str, decltype(sizeof(0)) len)
{
    return {};
}

void userdefined()
{
    "10"_literal.call(); // call works fine
    10_literal.call(); // error
    // ^ No matching literal operator for call to 'operator""_literal.call' with argument of type 'unsigned long long' or 'const char *', and no matching literal operator template
    // (it's parsed as '_a.call'  instead of '_a' - it should stop at the '.' like the StringLiteralParser does
    // is there an error in clang's NumericLiteralParse in clang/lib/Lex/LiteralSupport.cpp   ?
    
    (10_literal).call(); // call works fine
}


```
https://godbolt.org/z/vWhfzYbcc

It's parsed as '_literal.call'  instead of '_literal' - it should stop at the '.' like the StringLiteralParser does.
Is the error in clang's `NumericLiteralParser` in clang/lib/Lex/LiteralSupport.cpp   ?

</details>


---

