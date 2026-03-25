# #embed incorrectly assumes numbers are negative

**Author:** Nemi26
**URL:** https://github.com/llvm/llvm-project/issues/119256
**Status:** Closed
**Labels:** c23, clang:frontend, constexpr, embed
**Closed Date:** 2025-03-20T12:02:31Z

## Body

very hard to reproduce as I have no idea what triggers it. it should never ever happen though.

```cpp
static constexpr unsigned char data[] = {
#if __has_embed("data.bin")
#embed "data.bin"
#endif
};
```

`error: constexpr initializer evaluates to -41 which is not exactly representable in type 'const unsigned char'`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Streetware (StreetwareGames)

<details>
very hard to reproduce as I have no idea what triggers it. it should never ever happen though.

```cpp
static constexpr unsigned char data[] = {
#if __has_embed("data.bin")
#embed "data.bin"
#endif
};
```

`error: constexpr initializer evaluates to -41 which is not exactly representable in type 'const unsigned char'`
</details>


---

### Comment 2 - shafik

CC @AaronBallman @Fznamznon 

---

### Comment 3 - Fznamznon

per 6.10.4.1 p10
> Either form of the #embed directive specified previously behaves as specified later in this subclause.
The values of the integer constant expressions in the expanded sequence are determined by an
implementation-defined mapping of the resource’s data. Each integer constant expression’s value is
in the range from 0 to (2 embed element width) − 1, inclusive.200) 

Right, it probably shouldn't happen. It is hard to debug without a reproducer though.


---

### Comment 4 - Nemi26

I have a file with semi-random data that can be used to reproduce the bug. [rnd.txt](https://github.com/user-attachments/files/18080424/rnd.txt)


---

### Comment 5 - DownerCase

Yeah. It's getting caused by bytes with a value beyond 127...

You can reproduce, for example, with a file containing a single `ý`. It seems that the values will narrow to the expected value though if you suppress the warning.

---

### Comment 6 - cor3ntin

@Fznamznon https://godbolt.org/z/roWP89acd not quite the same symptom but you can see some incorrect values in the preprocessor output

---

### Comment 7 - Fznamznon

> [@Fznamznon](https://github.com/Fznamznon) https://godbolt.org/z/roWP89acd not quite the same symptom but you can see some incorrect values in the preprocessor output

Yeah, we have https://github.com/llvm/llvm-project/issues/102798 .

---

### Comment 8 - Fznamznon

> [@Fznamznon](https://github.com/Fznamznon) https://godbolt.org/z/roWP89acd not quite the same symptom but you can see some incorrect values in the preprocessor output

Yeah, all these come from messing up with strings. The binary data comes from a file in a for of a `StringRef`, need to make sure we always use bytes from there, not the values. I was able to fix 102798 doing that, but there is a problem with the fact that StringLiteral is used as internal data of EmbedExpr and we directly use it as an initializer if a single EmbedExpr appears in the initializer list of a char array. It is super fast but it is causing problems whenever we're checking values of this StringLiteral, like c23 constexpr initialization. I'm not sure what can be done except to invent some kind of BinaryLiteral which is like StringLiteral but is of unsinged char type and doesn't care about encoding. @cor3ntin @AaronBallman , WDYT?

---

### Comment 9 - AaronBallman

> I'm not sure what can be done except to invent some kind of BinaryLiteral which is like StringLiteral but is of unsinged char type and doesn't care about encoding. @cor3ntin @AaronBallman , WDYT?

Would another option be to add `Binary` to `StringLiteralKind`? It's a bit odd because these aren't really strings, but because we use `llvm::TrailingObjects` to store the actual data, we can't have a base class that represents a memory literal that subclasses then handle the type and encoding parts of, and a `BinaryLiteral` would be almost a copy of `StringLiteral` otherwise. (Maybe my idea isn't a great one, just wondering if you explored that as an option.)

---

### Comment 10 - Fznamznon

> Maybe my idea isn't a great one, just wondering if you explored that as an option.

No, I did not. Thanks!

---

