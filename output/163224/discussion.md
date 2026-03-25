# Bad recommended fix for nodiscard + enum class

**Author:** xTachyon
**URL:** https://github.com/llvm/llvm-project/issues/163224
**Status:** Closed
**Labels:** clang:frontend, confirmed, invalid-code-generation
**Closed Date:** 2025-12-03T14:20:14Z

## Body

[Link](https://godbolt.org/z/vb1dYo73q).

Code:
```cpp
[[nodiscard]] enum class E1 {};

enum [[nodiscard]] class E2 {};
```

Error:
```cpp
<source>:1:19: error: misplaced attributes; expected attributes here
    1 | [[nodiscard]] enum class E1 {};
      | ~~~~~~~~~~~~~     ^
      |                   [[nodiscard]]
<source>:3:6: error: an attribute list cannot appear here
    3 | enum [[nodiscard]] class E2 {};
      |      ^~~~~~~~~~~~~
```

For `E1`, clang says to put `[[nodiscard]]` after the `enum` keyword, but if you do that, it errors on not being valid there either.

My guess is that works with old `enum`s, but was not tested with `enum class`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Damian Andrei (xTachyon)

<details>
[Link](https://godbolt.org/z/vb1dYo73q).

Code:
```cpp
[[nodiscard]] enum class E1 {};

enum [[nodiscard]] class E2 {};
```

Error:
```cpp
&lt;source&gt;:1:19: error: misplaced attributes; expected attributes here
    1 | [[nodiscard]] enum class E1 {};
      | ~~~~~~~~~~~~~     ^
      |                   [[nodiscard]]
&lt;source&gt;:3:6: error: an attribute list cannot appear here
    3 | enum [[nodiscard]] class E2 {};
      |      ^~~~~~~~~~~~~
```

For `E1`, clang says to put `[[nodiscard]]` after the `enum` keyword, but if you do that, it errors on not being valid there either.

My guess is that works with old `enum`s, but was not tested with `enum class`.
</details>


---

### Comment 2 - erichkeane

The source of this diagnostic is the call to `ProhibitAttributes` in `Parser::ParseDeclOrFunctionDefInternal` in `Parser.cpp.` See ~1127.

We probably have to do a little parser-look-ahead there.  Right now we're doing it with 'string length', but we probably want to do the 'next token' location (unless of course it is enum class/struct, where we have to do TWO tokens ahead!).

But this is distinctly parser problems, so @AaronBallman  :)

Perhaps you have a better enough idea to mark this as 'good first issue'? 

---

### Comment 3 - AaronBallman

> Perhaps you have a better enough idea to mark this as 'good first issue'?

I don't have a better idea than that; I think lookahead is a reasonable way to do it because we only need to do a single token of lookahead. Should be pretty straightforward, though I suppose a potential oddity would be:
```
[[attribute]] enum<EOF>
```
where there are no tokens to look ahead to, but I'm not certain we can even hit this code path in that case because the code is guarded on finding a semi colon.

---

### Comment 4 - zygoloid

> Should be pretty straightforward, though I suppose a potential oddity would be:
> 
> ```
> [[attribute]] enum<EOF>
> ```

Even if we hit that, lookahead would produce an EOF token, which isn't `kw_class` / `kw_struct`, so we'd suggest moving the attributes after `enum` without needing to treat this as a special case.

---

